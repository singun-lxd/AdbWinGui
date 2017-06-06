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

void DeviceMonitor::GetDevices(std::vector<Device> vecDevice) const
{
	std::unique_lock<std::mutex> lock(m_lockDevices);
	// copy device data out
	for (const auto& pDevice : m_vecDevices)
	{
		vecDevice.push_back(*pDevice);
	}
}

AndroidDebugBridge* DeviceMonitor::GetServer() const
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

void DeviceMonitor::UpdateDevices(const DeviceVector& vecNew)
{
	m_lockDevices.lock();
	std::unique_ptr<DeviceListComparisonResult> result(DeviceListComparisonResult::Compare(m_vecDevices, vecNew));
	m_lockDevices.unlock();
	for (auto& device : *(result->m_pRemoved))
	{
		Device* pDevice = device.get();
		m_pServer->DeviceDisconnected(pDevice);

		m_lockDevices.lock();
		RemoveDevice(*pDevice);	// need call after callback
		m_lockDevices.unlock();
	}

	std::vector<Device*> newlyOnline;
	for (auto& entry : *(result->m_pUpdated))
	{
		// need to change state, cast const
		Device& device = const_cast<Device&>(entry.first);
		device.SetState(entry.second);
		device.Update(CHANGE_STATE);

		if (device.IsOnline())
		{
			newlyOnline.push_back(&device);
		}
	}

	for (auto& device : *(result->m_pAdded))
	{
		Device* pDevice = device.get();
		m_lockDevices.lock();
		m_vecDevices.push_back(device);
		m_lockDevices.unlock();

		m_pServer->DeviceConnected(pDevice);
		if (pDevice->IsOnline())
		{
			newlyOnline.push_back(pDevice);
		}
	}


	if (AndroidDebugBridge::GetClientSupport())
	{
// 		for (Device* device : newlyOnline)
//		{
// 			if (!StartMonitoringDevice(*device))
// 			{
// 			}
// 		}
	}

	for (const Device* pDevice : newlyOnline)
	{
		QueryAvdName(*pDevice);
	}
}

void DeviceMonitor::RemoveDevice(const Device& device)
{
	// lock m_vecDevices outside
	for (DeviceVector::iterator iter = m_vecDevices.begin(); iter != m_vecDevices.end(); )
	{
		const Device* pDevice = iter->get();
		if (_tcscmp(device.GetSerialNumber(), pDevice->GetSerialNumber()) == 0)
		{
			iter = m_vecDevices.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
}

DeviceVector::iterator DeviceMonitor::RemoveDevice(DeviceVector::iterator iterDevice)
{
	//device.ClearClientList();

	// lock m_vecDevices outside
	DeviceVector::iterator iter = m_vecDevices.erase(iterDevice);

// 	SocketClient client* = device.GetClientMonitoringSocket();
// 	if (client != NULL)
// 	{
// 		client->Close();
// 	}
	return iter;
}

void DeviceMonitor::QueryAvdName(const Device& device)
{
	if (!device.IsEmulator())
	{
		return;
	}

	// 	EmulatorConsole console = EmulatorConsole.getConsole(device);
	// 	if (console != null) {
	// 		device.setAvdName(console.getAvdName());
	// 		console.close();
	// 	}
}

SocketClient* DeviceMonitor::OpenAdbConnection()
{
	SocketClient* pClient = SocketClient::Open(AndroidDebugBridge::GetSocketAddress());
	pClient->SetTcpNoDelay(TRUE);
	return pClient;
}

void DeviceMonitor::ReleaseConnection()
{
	AdbHelper::ReleaseSocket();
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

//////////////////////////////////////////////////////////////////////////
// implements for DeviceListMonitorTask

DeviceMonitor::DeviceListMonitorTask::DeviceListMonitorTask(AndroidDebugBridge* pBridge, UpdateListener* pListener) : 
	m_pBridge(pBridge), m_pListener(pListener)
{
}

DeviceMonitor::DeviceListMonitorTask::~DeviceListMonitorTask()
{
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

		if (m_pAdbConnection != NULL && !m_bMonitoring)
		{
			m_bMonitoring = SendDeviceListMonitoringRequest();
			if (!m_bMonitoring)
			{
				int lastError = AdbHelper::GetLastError();
				if (lastError != ERROR_SUCCESS)
				{
					HandleErrorInMonitorLoop(lastError);
				}
			}
		}

		if (m_bMonitoring)
		{
			int length = ReadLength(m_pAdbConnection, m_szBuffer, _countof(m_szBuffer) - 1);

			if (length >= 0)
			{
				// read the incoming message
				ProcessIncomingDeviceData(length);

				// flag the fact that we have build the list at least once.
				m_bInitialDeviceListDone = true;
			}
		}
	} while (!m_bQuit);
}

bool DeviceMonitor::DeviceListMonitorTask::SendDeviceListMonitoringRequest()
{
	bool bRet = true;
 	std::unique_ptr<const char[]> request(AdbHelper::FormAdbRequest(ADB_TRACK_DEVICES_COMMAND));

 	bool bWrite = AdbHelper::Write(m_pAdbConnection, request.get());
	if (bWrite)
	{
		AdbHelper::AdbResponse* resp = AdbHelper::ReadAdbResponse(m_pAdbConnection, false);
		if (resp == NULL || !resp->okay)
		{
			// request was refused by adb!
			bRet = false;
		}
		if (resp != NULL)
		{
			delete resp;
		}
	}

	return bRet;
}

void DeviceMonitor::DeviceListMonitorTask::HandleErrorInMonitorLoop(int errorCode)
{
	if (!m_bQuit)
	{
		m_bMonitoring = false;
		if (m_pAdbConnection != NULL)
		{
			m_pAdbConnection->Close();
			delete m_pAdbConnection;
			m_pAdbConnection = NULL;

			m_pListener->ConnectionError(errorCode);
		}
	}
}

void DeviceMonitor::DeviceListMonitorTask::ProcessIncomingDeviceData(int length)
{
	std::map<std::tstring, IDevice::DeviceState> result;
	if (length > 0)
	{
		size_t buffSize = length + 1;
		std::unique_ptr<char[]> buffer(new char[buffSize]);
		ZeroMemory(buffer.get(), buffSize * sizeof(char));
		// read adb response and parse output
		const char* response = Read(m_pAdbConnection, buffer.get(), length);
		ParseDeviceListResponse(response, result);
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

bool DeviceMonitor::DeviceListMonitorTask::IsMonitoring() const
{
	return m_bMonitoring;
}

bool DeviceMonitor::DeviceListMonitorTask::HasInitialDeviceList() const
{
	return m_bInitialDeviceListDone;
}

int DeviceMonitor::DeviceListMonitorTask::GetConnectionAttemptCount() const
{
	return m_nConnectionAttempt;
}

int DeviceMonitor::DeviceListMonitorTask::GetRestartAttemptCount() const
{
	return m_nRestartAttemptCount;
}

void DeviceMonitor::DeviceListMonitorTask::Stop()
{
	m_bQuit = true;
}

//////////////////////////////////////////////////////////////////////////
// implements for DeviceListUpdateListener

DeviceMonitor::DeviceListUpdateListener::DeviceListUpdateListener(DeviceMonitor* pMonitor) :
	m_pMonitor(pMonitor)
{
}

DeviceMonitor::DeviceListUpdateListener::~DeviceListUpdateListener()
{
}

void DeviceMonitor::DeviceListUpdateListener::ConnectionError(int errorCode)
{
	std::unique_lock<std::mutex> lock(m_pMonitor->m_lockDevices);
	DeviceVector& vecDevice = m_pMonitor->m_vecDevices;
	for (DeviceVector::iterator iter = vecDevice.begin(); iter != vecDevice.end();)
	{
		const Device* pDevice = iter->get();
		m_pMonitor->m_pServer->DeviceDisconnected(pDevice);
		iter = m_pMonitor->RemoveDevice(iter);	// need call after callback
	}
}

void DeviceMonitor::DeviceListUpdateListener::DeviceListUpdate(const std::map<std::tstring, IDevice::DeviceState>& devices)
{
	DeviceVector vec;
	for (auto &iter : devices)
	{
		std::shared_ptr<Device> dev(new Device(m_pMonitor, iter.first.c_str(), iter.second));
		vec.push_back(dev);
	}
	// now merge the new devices with the old ones.
	m_pMonitor->UpdateDevices(vec);
}

//////////////////////////////////////////////////////////////////////////
// implements for DeviceListComparisonResult

DeviceMonitor::DeviceListComparisonResult::DeviceListComparisonResult(std::map<Device, IDevice::DeviceState>* updated,
	DeviceVector* added, DeviceVector* removed) :
	m_pUpdated(updated), m_pAdded(added), m_pRemoved(removed)
{
}

DeviceVector::iterator DeviceMonitor::DeviceListComparisonResult::Find(DeviceVector& devices, const Device& device)
{
	DeviceVector::iterator iter;
	for (iter = devices.begin(); iter != devices.end(); iter++)
	{
		Device* pDevice = iter->get();
		if (_tcscmp(pDevice->GetSerialNumber(), device.GetSerialNumber()) == 0)
		{
			return iter;
		}
	}
	return devices.end();
}

DeviceMonitor::DeviceListComparisonResult* DeviceMonitor::DeviceListComparisonResult::Compare(
	const DeviceVector& previous, const DeviceVector& current)
{
	DeviceVector curCopy;
	curCopy.assign(current.begin(), current.end());

	std::map<Device, IDevice::DeviceState>* pUpdated = new std::map<Device, IDevice::DeviceState>;
	DeviceVector* pAdded = new DeviceVector;
	DeviceVector* pRemoved = new DeviceVector;

	for (const auto& pDevice : previous)
	{
		const Device& device = *pDevice;
		DeviceVector::iterator iter = Find(curCopy, device);
		if (iter != curCopy.end())
		{
			Device* pDevice = iter->get();
			if (pDevice->GetState() != device.GetState())
			{
				(*pUpdated)[device] = pDevice->GetState();
			}
			curCopy.erase(iter);
		}
		else
		{
			pRemoved->push_back(pDevice);
		}
	}

	pAdded->assign(curCopy.begin(), curCopy.end());

	return new DeviceListComparisonResult(pUpdated, pAdded, pRemoved);
}