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

#include "System/SysDef.h"
#include "IDevice.h"
#include "DeviceMonnitor.h"

// define class
class DeviceMonitor;

class Device : public IDevice
{
private:
	static const long s_lInstallTimeOut;
	static long GetInstallTimeOut();

private:
	const DeviceMonitor* m_pMonitor;
	std::tstring m_strSerialNumber;
	DeviceState m_stateDev = UNKNOWN;
	
public:
	Device();
	Device(const IDevice* pDevice);
	Device(DeviceMonitor* monitor, const TString serialNumber, DeviceState deviceState);
	bool Device::operator < (const Device &) const;
	bool Device::operator == (const Device &) const;

public:
	virtual const TString GetName() const override;
	virtual void ExecuteShellCommand(const TString command, const IShellOutputReceiver& receiver, long timeOut) override;
	virtual std::future<std::tstring> GetSystemProperty(const std::tstring& name) const override;

	virtual const TString GetSerialNumber() const override;
	virtual DeviceState GetState() const override;
	void SetState(DeviceState state);
	virtual bool IsOnline() const override;
	virtual bool IsEmulator() const override;
	virtual bool IsOffline() const override;
	virtual bool IsBootLoader() const override;

	void Update(int changeMask);
};
