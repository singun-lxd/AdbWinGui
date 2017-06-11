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
#include "IDevice.h"
#include "DeviceMonnitor.h"
#include "../System/SocketClient.h"
#include "MultiLineReceiver.h"

// define class
class DeviceMonitor;
class SyncService;

class Device : public IDevice
{
private:
	static const long s_lInstallTimeOut;
	static long GetInstallTimeOut();

	class InstallReceiver : public MultiLineReceiver
	{
	private:
		#define SUCCESS_OUTPUT  "Success"
		#define FAILURE_PATTERN "Failure\\s+\\[(.*)\\]"
		std::tstring m_strErrorMessage;

	public:
		virtual void ProcessNewLines(const std::vector<std::string>& vecArray) override;
		virtual bool IsCancelled() override;
		const TString GetErrorMessage();
	};

private:
	const DeviceMonitor* m_pMonitor;
	std::tstring m_strSerialNumber;
	DeviceState m_stateDev = UNKNOWN;
	SocketClient* m_pSocketClient;
	int m_nApiLevel;
	
public:
	Device();
	Device(const IDevice* pDevice);
	Device(DeviceMonitor* monitor, const TString serialNumber, DeviceState deviceState);
	~Device();
	bool Device::operator < (const Device &) const;
	bool Device::operator == (const Device &) const;

public:
	virtual const TString GetName() const override;
	virtual int ExecuteShellCommand(const TString command, IShellOutputReceiver* receiver, long timeOut) override;
	virtual std::future<std::tstring> GetSystemProperty(const std::tstring& name) const override;

	virtual const TString GetSerialNumber() const override;
	virtual DeviceState GetState() const override;
	void SetState(DeviceState state);
	virtual const TString GetProperty(const TString name) const override;
	virtual bool IsOnline() const override;
	virtual bool IsEmulator() const override;
	virtual bool IsOffline() const override;
	virtual bool IsBootLoader() const override;
	SyncService* GetSyncService();
	virtual int PushFile(const TString local, const TString remote) override;
	virtual int PullFile(const TString remote, const TString local) override;
	virtual int InstallPackage(const TString packageFilePath, bool reinstall, const TString args[] = NULL, int argCount = 0) override;
	virtual int InstallPackages(const TString apkFilePaths[], int apkCount, int timeOutInMs, bool reinstall,
		const TString args[] = NULL, int argCount = 0) override;
	virtual int SyncPackageToDevice(const TString localFilePath, std::tstring& remotePath) override;
	virtual int InstallRemotePackage(const TString remoteFilePath, bool reinstall, const TString args[] = NULL, int argCount = 0) override;
	virtual int RemoveRemotePackage(const TString remoteFilePath) override;
	virtual int UninstallPackage(const TString packageName) override;

	void SetClientMonitoringSocket(SocketClient* socketClient);
	SocketClient* GetClientMonitoringSocket();
	void Update(int changeMask);

private:
	int GetApiLevel();
	static const TString GetFileName(const TString filePath);
};
