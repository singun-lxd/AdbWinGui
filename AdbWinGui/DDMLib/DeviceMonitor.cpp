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
// 			int length = readLength(mAdbConnection, mLengthBuffer);
// 
// 			if (length >= 0) {
// 				// read the incoming message
// 				processIncomingDeviceData(length);
// 
// 				// flag the fact that we have build the list at least once.
// 				m_bInitialDeviceListDone = true;
// 			}
		}
	} while (!m_bQuit);
}

bool DeviceMonitor::DeviceListMonitorTask::SendDeviceListMonitoringRequest()
{
 	const byte* request = AdbHelper::FormAdbRequest(ADB_TRACK_DEVICES_COMMAND);

 	bool bWrite = AdbHelper::Write(m_pAdbConnection, request);
	if (bWrite)
	{
		delete[] request;
		return false;
	}
// 	AdbResponse resp = AdbHelper.readAdbResponse(mAdbConnection, false);
// 	if (!resp.okay)
// 	{
// 		// request was refused by adb!
// 		return false;
// 	}
	delete[] request;
	return true;
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