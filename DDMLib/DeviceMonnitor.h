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

#include "CommonDefine.h"
#include "AndroidDebugBridge.h"
#include "IDevice.h"
#include "Device.h"
#include "System/SocketClient.h"

// define class
class AndroidDebugBridge;
class Device;

typedef std::vector<std::shared_ptr<Device>> DeviceVector;

class DeviceMonitor
{
private:
	class DeviceListMonitorTask
	{
	public:
		interface UpdateListener
		{
			virtual void ConnectionError(int errorCode) = 0;
			virtual void DeviceListUpdate(const std::map<std::tstring, IDevice::DeviceState>& devices) = 0;
		};
	private:
		char m_szBuffer[5] = { 0 };
		AndroidDebugBridge* const m_pBridge;
		std::unique_ptr<UpdateListener> m_pListener;	// need to free listener

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
		void HandleErrorInMonitorLoop(int errorCode);
		void ProcessIncomingDeviceData(int length);
		static void ParseDeviceListResponse(const char* result,
			std::map<std::tstring, IDevice::DeviceState>& list);
		bool IsMonitoring() const;
		bool HasInitialDeviceList() const;
		int GetConnectionAttemptCount() const;
		int GetRestartAttemptCount() const;
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
		virtual void DeviceListUpdate(const std::map<std::tstring, IDevice::DeviceState>& devices);
	};

	class DeviceListComparisonResult
	{
	public:
		std::unique_ptr<std::map<Device, IDevice::DeviceState>> m_pUpdated;
		std::unique_ptr<DeviceVector> m_pAdded;
		std::unique_ptr<DeviceVector> m_pRemoved;

	private:
		DeviceListComparisonResult(std::map<Device, IDevice::DeviceState>* updated,
			DeviceVector* added, DeviceVector* removed);
		static DeviceVector::iterator Find(DeviceVector& devices, const Device& device);

	public:
		static DeviceListComparisonResult* Compare(const DeviceVector& previous,
			const DeviceVector& current);
	};

private:
	AndroidDebugBridge* m_pServer;
	DeviceListMonitorTask* m_pDeviceListMonitorTask;
	std::future<void> m_taskMonitor;
	DeviceVector m_vecDevices;
	mutable std::mutex m_lockDevices;

public:
	DeviceMonitor(AndroidDebugBridge* pServer);
	~DeviceMonitor();

	bool m_bQuit = false;

	void GetDevices(std::vector<Device>& vecDevice) const;
	AndroidDebugBridge* GetServer() const;

	void Start();
	void Stop();

	void UpdateDevices(const DeviceVector& veNew);
	void RemoveDevice(const Device& device);
	DeviceVector::iterator RemoveDevice(DeviceVector::iterator iterDevice);

private:
	static void QueryAvdName(const Device& device);

public:
	static SocketClient* OpenAdbConnection();
	static void ReleaseConnection();

	static int ReadLength(SocketClient* socket, char* buffer, int length);
	static char* Read(SocketClient* socket, char* buffer, int length);
};
