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
#include "DeviceMonnitor.h"

class DeviceMonitor;	// define class

class AndroidDebugBridge
{
private:
	static AndroidDebugBridge* s_pThis;
	std::tstring m_AdbLocation;

	bool m_bVersionCheck;
	bool m_bStarted;
	DeviceMonitor* m_pDeviceMonitor;

private:
	AndroidDebugBridge(const TCHAR* szLocation);
	void CheckAdbVersion();

	static void GetAdbVersion(const std::tstring& adb, std::tstring& version);
public:
	static AndroidDebugBridge& CreateBridge(const TCHAR* szLocation, bool forceNewBridge = false);
	static AndroidDebugBridge& GetBridge();

	boolean Start();
	boolean Stop();
};