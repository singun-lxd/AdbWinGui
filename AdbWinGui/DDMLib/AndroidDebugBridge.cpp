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

#include <iostream>
#include <future>
#include <thread>
#include "AndroidDebugBridge.h"

#define MIN_ADB_VERSION _T("1.2.0")

AndroidDebugBridge* AndroidDebugBridge::s_pThis = NULL;

AndroidDebugBridge::AndroidDebugBridge(const TCHAR* szLocation)
{
	m_bStarted = false;
	m_pDeviceMonitor = NULL;

	CheckAdbVersion();
}

void AndroidDebugBridge::CheckAdbVersion()
{
	// default is bad check
	m_bVersionCheck = false;

	if (m_AdbLocation.empty()) {
		return;
	}

	std::tstring ver;
	GetAdbVersion(m_AdbLocation, ver);
	return;
}

void AndroidDebugBridge::GetAdbVersion(const std::tstring& adb, std::tstring& version)
{
	// test code for thread creation
	std::packaged_task<int(int, int)> taskVer([](int a, int b) -> int
	{
		return a + b;
	});
	std::future<int> futureVer = taskVer.get_future();
	std::thread(std::move(taskVer), 1ul, 2ul).detach();
	int result = futureVer.get();

}

AndroidDebugBridge& AndroidDebugBridge::CreateBridge(const TCHAR* szLocation, bool forceNewBridge)
{
	if (s_pThis != NULL) {
		if (!s_pThis->m_AdbLocation.empty() && s_pThis->m_AdbLocation.compare(szLocation) == 0 &&
			!forceNewBridge) {
			return *s_pThis;
		}
		else {
			// stop the current server
			s_pThis->Stop();
		}
	}
	s_pThis = new AndroidDebugBridge(szLocation);
	s_pThis->Start();
	return *s_pThis;
}

AndroidDebugBridge& AndroidDebugBridge::GetBridge()
{
	return *s_pThis;
}

boolean AndroidDebugBridge::Start()
{
	return true;
}

boolean AndroidDebugBridge::Stop()
{
	return true;
}
