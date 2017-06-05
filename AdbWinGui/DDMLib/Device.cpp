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

#include "Device.h"
#include "AndroidEnvVar.h"

#define GET_PROP_TIMEOUT_MS				100
#define INSTALL_TIMEOUT_MINUTES			Device::s_lInstallTimeOut

const long Device::s_lInstallTimeOut = Device::GetInstallTimeOut();

long Device::GetInstallTimeOut()
{
	AndroidEnvVar env;
	long timeOut = env.GetInstallTimeOut();
	if (timeOut < 0)
	{
		// use default value
		timeOut = 4;
	}
	return timeOut;
}

Device::Device()
{
}

Device::Device(DeviceMonitor* monitor, const TString serialNumber, DeviceState deviceState) :
	m_pMonitor(monitor),
	m_strSerialNumber(serialNumber),
	m_stateDev(deviceState)
{
}
const TString Device::GetName() const
{
	return NULL;
}

void Device::ExecuteShellCommand(const TString command, const IShellOutputReceiver& receiver, long timeOut)
{

}

std::future<std::tstring> Device::GetSystemProperty(const std::tstring& name) const
{
	return std::future<std::tstring>();
}

const TString Device::GetSerialNumber() const
{
	return m_strSerialNumber.c_str();
}

IDevice::DeviceState Device::GetState() const
{
	return m_stateDev;
}
