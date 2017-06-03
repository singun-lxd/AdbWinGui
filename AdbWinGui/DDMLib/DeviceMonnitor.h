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

#pragma once

#include <map>
#include "CommonDefine.h"
#include "AndroidDebugBridge.h"
#include "IDevice.h"
#include "System/SocketClient.h"

class AndroidDebugBridge;	// define class

class DeviceMonitor
{
private:
	class DeviceListMonitorTask
	{
	public:
		interface UpdateListener
		{
			virtual void ConnectionError(int errorCode) = 0;
			virtual void DeviceListUpdate(std::map<std::tstring, IDevice::DeviceState>& devices) = 0;
		};
	private:
		AndroidDebugBridge* const m_pBridge;
		UpdateListener* const m_pListener;

		SocketClient* m_pAdbConnection = NULL;
		bool m_bMonitoring = false;
		int m_nConnectionAttempt = 0;
		int m_nRestartAttemptCount = 0;
		bool m_bInitialDeviceListDone = false;

		bool m_bQuit = false;
	public:
		DeviceListMonitorTask(AndroidDebugBridge* pBridge, UpdateListener* pListener);
		~DeviceListMonitorTask();

		void Run();
		bool SendDeviceListMonitoringRequest();
		bool IsMonitoring();
		bool HasInitialDeviceList();
		int GetConnectionAttemptCount();
		int GetRestartAttemptCount();
		void Stop();
	};

	class DeviceListUpdateListener : public DeviceListMonitorTask::UpdateListener
	{
	private:
		DeviceMonitor* const m_pMonitor;
	public:
		DeviceListUpdateListener(DeviceMonitor* pMonitor);
		~DeviceListUpdateListener();

		virtual void ConnectionError(int errorCode);
		virtual void DeviceListUpdate(std::map<std::tstring, IDevice::DeviceState>& devices);
	};

private:
	AndroidDebugBridge* m_pServer;
	DeviceListMonitorTask* m_pDeviceListMonitorTask;

public:
	DeviceMonitor(AndroidDebugBridge* pServer);
	~DeviceMonitor();

	bool m_bQuit = false;

	const IDevice* GetDevices();
	AndroidDebugBridge* GetServer();

	void Start();
	void Stop();

public:
	static SocketClient* OpenAdbConnection();
	static void ReleaseConnection();
};
