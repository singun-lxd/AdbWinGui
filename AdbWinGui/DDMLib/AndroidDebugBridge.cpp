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
#include "StringUtils.h"
#include "System/Process.h"
#include "System/StreamReader.h"
#include "../Utils/AndroidEnvVar.h"

#define MIN_ADB_VERSION    _T("1.0.20")
#define DEFAULT_ADB_HOST   _T("127.0.0.1")
#define DEFAULT_ADB_PORT   5037

AdbVersion* AndroidDebugBridge::s_pCurVersion = NULL;
AndroidDebugBridge* AndroidDebugBridge::s_pThis = NULL;
bool AndroidDebugBridge::s_bInitialized = false;
bool AndroidDebugBridge::s_bClientSupport = false;
int AndroidDebugBridge::s_nAdbServerPort = 0;
SocketAddress AndroidDebugBridge::s_addSocket;

AndroidDebugBridge::AndroidDebugBridge(const TString szLocation)
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
	AdbVersion* pVersion = GetAdbVersion(m_AdbLocation.c_str());
	if (pVersion != NULL && *pVersion > *s_pCurVersion)
	{
		// version check succeed
		m_bVersionCheck = true;
	}
	delete pVersion;
	return;
}

AdbVersion* AndroidDebugBridge::GetAdbVersion(const TString adb)
{
	std::packaged_task<AdbVersion*()> taskVer([&adb]() -> AdbVersion*
	{
		Process procAdb(adb);
		procAdb.OpenReadWrite();
		procAdb.Start();
		procAdb.CloseWrite();

		// read process output
		const int BUFFER_SIZE = 1024;
		CharStreamReader frProcess(procAdb.GetRead(), BUFFER_SIZE);
		std::tstringstream& tss = frProcess.ReadData();

		// parse adb version results
		AdbVersion* pVersion = NULL;
		std::tstring line;
		while (std::getline(tss, line)) {
			StringUtils::TrimString(line);
			pVersion = AdbVersion::ParseFrom(line.c_str());
			if (pVersion != AdbVersion::UNKNOWN)
			{
				break;
			}
		}
		procAdb.CloseRead();

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

AndroidDebugBridge& AndroidDebugBridge::CreateBridge(const TString szLocation, bool forceNewBridge)
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

void AndroidDebugBridge::InitIfNeeded(bool clientSupport)
{
	if (s_bInitialized) {
		return;
	}

	Init(clientSupport);
}

void AndroidDebugBridge::Init(bool clientSupport)
{
	if (s_bInitialized) {
		return;
	}
	s_bInitialized = true;
	s_bClientSupport = clientSupport;

	// Determine port and instantiate socket address.
	InitAdbSocketAddr();
}

void AndroidDebugBridge::InitAdbSocketAddr()
{
	s_nAdbServerPort = GetAdbServerPort();
	s_addSocket.SetSocketAddress(DEFAULT_ADB_HOST);
	s_addSocket.SetSocketPort(s_nAdbServerPort);
}

int AndroidDebugBridge::GetAdbServerPort()
{
	AndroidEnvVar envVar;
	int nPort = envVar.GetAdbServerPort();
	if (nPort <= 0 || nPort >= 65535)
	{
		// invalid port, use default
		return DEFAULT_ADB_PORT;
	}
	return nPort;
}

const IDevice* AndroidDebugBridge::getDevices()
{
	return NULL;
}

bool AndroidDebugBridge::Start()
{
	return true;
}

bool AndroidDebugBridge::Stop()
{
	return true;
}
