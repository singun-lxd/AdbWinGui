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

	if (m_pDeviceListMonitorTask != NULL) {
		m_pDeviceListMonitorTask->Stop();
	}
}

SocketClient* DeviceMonitor::OpenAdbConnection()
{
	return NULL;
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
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			else
			{
				m_nConnectionAttempt = 0;
			}			
		}

		// todo run monitor here

	} while (m_bQuit);
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