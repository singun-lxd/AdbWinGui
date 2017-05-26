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
#include "Utils.h"

#define MIN_ADB_VERSION _T("1.0.20")

AdbVersion* AndroidDebugBridge::s_pCurVersion;
AndroidDebugBridge* AndroidDebugBridge::s_pThis = NULL;

AndroidDebugBridge::AndroidDebugBridge(const TCHAR* szLocation)
{
	m_bStarted = false;
	m_pDeviceMonitor = NULL;
	m_AdbLocation = szLocation;

	CheckAdbVersion();
}

void AndroidDebugBridge::CheckAdbVersion()
{
	// default is bad check
	m_bVersionCheck = false;

	if (m_AdbLocation.empty()) {
		return;
	}

	if (s_pCurVersion == NULL)
	{
		s_pCurVersion = AdbVersion::ParseFrom(MIN_ADB_VERSION);
	}
	AdbVersion* pVersion = GetAdbVersion(m_AdbLocation);
	if (pVersion != NULL && *pVersion > *s_pCurVersion)
	{
		// version check succeed
		m_bVersionCheck = true;
	}
	delete pVersion;
	return;
}

AdbVersion* AndroidDebugBridge::GetAdbVersion(const std::tstring& adb)
{
	std::packaged_task<AdbVersion*()> taskVer([&adb]() -> AdbVersion*
	{
		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
		HANDLE hRead, hWrite;
		if (!::CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			return NULL;
		}

		// run adb process
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		::GetStartupInfo(&si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;

		PROCESS_INFORMATION pi;
		if (!::CreateProcess(NULL, const_cast<LPTSTR>(adb.c_str()), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			::CloseHandle(hRead);
			::CloseHandle(hWrite);
			return NULL;
		}

		::CloseHandle(hWrite);

		// read process output
		const int BUFFER_SIZE = 1024;
		std::tstringstream tss;
		CHAR buff[BUFFER_SIZE] = { 0 };
		DWORD dwRead = 0;
		while (::ReadFile(hRead, buff, BUFFER_SIZE, &dwRead, NULL))
		{
			tss << buff;
		}
		::CloseHandle(hRead);

		// parse adb version results
		AdbVersion* pVersion = NULL;
		std::tstring line;
		while (std::getline(tss, line)) {
			Utils::TrimString(line);
			pVersion = AdbVersion::ParseFrom(line.c_str());
			if (pVersion != AdbVersion::UNKNOWN)
			{
				break;
			}
		}

		return pVersion;
	});
	
	AdbVersion* pRet = NULL;
	std::future<AdbVersion*> futureVer = taskVer.get_future();
	std::thread(std::move(taskVer)).detach();
	std::future_status status = futureVer.wait_for(std::chrono::seconds(5));
	if (status == std::future_status::ready)
	{
		pRet = futureVer.get();
	}
	return pRet;
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