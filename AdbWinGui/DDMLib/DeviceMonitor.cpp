/*
AdbWinGui (Android Debug Bridge Windows GUI)
Copyright (C) 2017  singun

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DeviceMonnitor.h"
#include "AdbHelper.h"

#define ADB_TRACK_DEVICES_COMMAND	"host:track-devices"

DeviceMonitor::DeviceMonitor(AndroidDebugBridge* pServer)
{
	m_pServer = pServer;
}

DeviceMonitor::~DeviceMonitor()
{
	m_pServer = NULL;
	if (m_pDeviceListMonitorTask != NULL)
	{
		delete m_pDeviceListMonitorTask;
		m_pDeviceListMonitorTask = NULL;
	}
}

const IDevice* DeviceMonitor::GetDevices()
{
	return NULL;
}

AndroidDebugBridge* DeviceMonitor::GetServer()
{
	return m_pServer;
}

void DeviceMonitor::Start()
{
	m_pDeviceListMonitorTask = new DeviceListMonitorTask(m_pServer, new DeviceListUpdateListener(this));
	std::thread tdMonitor(std::bind(&DeviceListMonitorTask::Run, m_pDeviceListMonitorTask));
	tdMonitor.detach();
}

void DeviceMonitor::Stop()
{
	m_bQuit = true;

	if (m_pDeviceListMonitorTask != NULL)
	{
		m_pDeviceListMonitorTask->Stop();
	}
}

SocketClient* DeviceMonitor::OpenAdbConnection()
{
	SocketClient* pClient = SocketClient::Open(AndroidDebugBridge::GetSocketAddress());
	pClient->SetTcpNoDelay(TRUE);
	return pClient;
}

void DeviceMonitor::ReleaseConnection()
{
	SocketClient::Release();
}

int DeviceMonitor::ReadLength(SocketClient* socket, char* buffer, int length)
{
	int len = -1;
	char* msg = Read(socket, buffer, length);
	if (msg != NULL)
	{
		std::istringstream iss(msg);
		iss >> std::hex >> len;
	}
	return len;
}

char* DeviceMonitor::Read(SocketClient* socket, char* buffer, int length)
{
	int readCount = 0;
	while (readCount < length)
	{
		int count;
		count = socket->Read(buffer, length);
		readCount += count;
		if (count < 0)
		{
			return NULL;
		}
	}
	return buffer;
}

DeviceMonitor::DeviceListMonitorTask::DeviceListMonitorTask(AndroidDebugBridge* pBridge, UpdateListener* pListener) : 
	m_pBridge(pBridge), m_pListener(pListener)
{
}

DeviceMonitor::DeviceListMonitorTask::~DeviceListMonitorTask()
{
	// need to free listener
	if (m_pListener != NULL)
	{
		delete m_pListener;
	}

	if (m_pAdbConnection != NULL)
	{
		delete m_pAdbConnection;
		m_pAdbConnection = NULL;
	}
}

void DeviceMonitor::DeviceListMonitorTask::Run()
{
	do
	{
		if (m_pAdbConnection == NULL)
		{
			m_pAdbConnection = OpenAdbConnection();
			if (m_pAdbConnection == NULL)
			{
				m_nConnectionAttempt++;
				if (m_nConnectionAttempt > 10)
				{
					if (!m_pBridge->StartAdb())
					{
						m_nRestartAttemptCount++;
					}
					else
					{
						m_nRestartAttemptCount = 0;
					}
				}
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			else
			{
				m_nConnectionAttempt = 0;
			}			
		}

		if (m_pAdbConnection != NULL && !m_bMonitoring) {
			m_bMonitoring = SendDeviceListMonitoringRequest();
		}

		if (m_bMonitoring) {
			int length = ReadLength(m_pAdbConnection, m_szBuffer, _countof(m_szBuffer) - 1);

			if (length >= 0) {
				// read the incoming message
				ProcessIncomingDeviceData(length);

				// flag the fact that we have build the list at least once.
				m_bInitialDeviceListDone = true;
			}
		}
		break;//todo
	} while (!m_bQuit);
}

bool DeviceMonitor::DeviceListMonitorTask::SendDeviceListMonitoringRequest()
{
	bool bRet = true;
 	const char* request = AdbHelper::FormAdbRequest(ADB_TRACK_DEVICES_COMMAND);
	AdbHelper::AdbResponse* resp = NULL;

 	bool bWrite = AdbHelper::Write(m_pAdbConnection, request);
	if (bWrite)
	{
		resp = AdbHelper::ReadAdbResponse(m_pAdbConnection, false);
		if (resp == NULL || !resp->okay)
		{
			// request was refused by adb!
			bRet = false;
		}
	}

	if (resp != NULL)
	{
		delete resp;
	}
	delete[] request;
	return bRet;
}

void DeviceMonitor::DeviceListMonitorTask::ProcessIncomingDeviceData(int length)
{
	std::map<std::tstring, IDevice::DeviceState> result;
	if (length > 0)
	{
		size_t buffSize = length + 1;
		char* buffer = new char[buffSize];
		ZeroMemory(buffer, buffSize * sizeof(char));
		// read adb response and parse output
		const char* response = Read(m_pAdbConnection, buffer, length);
		ParseDeviceListResponse(response, result);
		delete[] buffer;
	}

	m_pListener->DeviceListUpdate(result);
}

void DeviceMonitor::DeviceListMonitorTask::ParseDeviceListResponse(const char* result,
	std::map<std::tstring, IDevice::DeviceState>& list)
{
	// convert to tchar stream
	std::tstringstream iss;
	iss << result;

	std::tstring line;
	while (std::getline(iss, line))
	{
		// split each line with \t char
		const TCHAR delim = _T('\t');
		size_t pos = line.find(delim);
		size_t lineLen = line.length();
		if (pos != std::tstring::npos && pos < lineLen - 1)
		{
			std::tstring device = line.substr(0, pos);
			std::tstring deviceState = line.substr(pos + 1);
			IDevice::DeviceState state = IDevice::GetState(deviceState.c_str());
			// insert a device
			list[device] = state;
		}
	}
}

bool DeviceMonitor::DeviceListMonitorTask::IsMonitoring()
{
	return m_bMonitoring;
}

bool DeviceMonitor::DeviceListMonitorTask::HasInitialDeviceList()
{
	return m_bInitialDeviceListDone;
}

int DeviceMonitor::DeviceListMonitorTask::GetConnectionAttemptCount()
{
	return m_nConnectionAttempt;
}

int DeviceMonitor::DeviceListMonitorTask::GetRestartAttemptCount()
{
	return m_nRestartAttemptCount;
}

void DeviceMonitor::DeviceListMonitorTask::Stop()
{
	m_bQuit = true;
}

DeviceMonitor::DeviceListUpdateListener::DeviceListUpdateListener(DeviceMonitor* pMonitor) :
	m_pMonitor(pMonitor)
{
}

DeviceMonitor::DeviceListUpdateListener::~DeviceListUpdateListener()
{
}

void DeviceMonitor::DeviceListUpdateListener::ConnectionError(int errorCode)
{

}

void DeviceMonitor::DeviceListUpdateListener::DeviceListUpdate(std::map<std::tstring, IDevice::DeviceState>& devices)
{

}