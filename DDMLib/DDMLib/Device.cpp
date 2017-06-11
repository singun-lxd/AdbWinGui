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
#include <regex>
#include "AndroidEnvVar.h"
#include "../System/File.h"
#include "SyncService.h"
#include "AndroidDebugBridge.h"
#include "Log.h"
#include "AdbHelper.h"
#include "NullOutputReceiver.h"

#define GET_PROP_TIMEOUT_MS				100
#define INSTALL_TIMEOUT_MINUTES			Device::s_lInstallTimeOut

#define DEVICE								_T("device")

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

Device::Device() : m_pMonitor(NULL), m_pSocketClient(NULL)
{
	m_nApiLevel = 0;
}

Device::Device(DeviceMonitor* monitor, const TString serialNumber, DeviceState deviceState) :
	m_pMonitor(monitor),
	m_strSerialNumber(serialNumber),
	m_stateDev(deviceState),
	m_pSocketClient(NULL)
{
	m_nApiLevel = 0;
}

Device::Device(const IDevice* pDevice) : m_pMonitor(NULL), m_pSocketClient(NULL)
{
	m_strSerialNumber = pDevice->GetSerialNumber();
	m_stateDev = pDevice->GetState();
	m_nApiLevel = 0;
}

Device::~Device()
{
	// need to free socket client
	if (m_pSocketClient != NULL)
	{
		m_pSocketClient->Close();
		delete m_pSocketClient;
		m_pSocketClient = NULL;
	}
}

bool Device::operator < (const Device& r) const
{
	return m_strSerialNumber < r.m_strSerialNumber;
}

bool Device::operator==(const Device & r) const
{
	if (m_strSerialNumber.length() != r.m_strSerialNumber.length())
	{
		return false;
	}
	return _tcscmp(m_strSerialNumber.c_str(), r.m_strSerialNumber.c_str()) == 0;
}

const TString Device::GetName() const
{
	return NULL;
}

int Device::ExecuteShellCommand(const TString command, IShellOutputReceiver* receiver, long timeOut)
{
	return AdbHelper::ExecuteRemoteCommand(AndroidDebugBridge::GetSocketAddress(), command, this,
		receiver, timeOut);
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

int Device::GetApiLevel()
{
	if (m_nApiLevel > 0) {
		return m_nApiLevel;
	}

	const TString buildApi = GetProperty(PROP_BUILD_API_LEVEL);
	m_nApiLevel = buildApi == NULL ? -1 : _ttoi(buildApi);
	return m_nApiLevel;
}

int Device::InstallPackage(const TString packageFilePath, bool reinstall, const TString args[], int argCount)
{
	int nRetCode = -1;
	std::tstring remoteFilePath;
	nRetCode = SyncPackageToDevice(packageFilePath, remoteFilePath);
	if (nRetCode == 0)
	{
		nRetCode = InstallRemotePackage(remoteFilePath.c_str(), reinstall, args, argCount);
		RemoveRemotePackage(remoteFilePath.c_str());
	}
	return nRetCode;
}

int Device::InstallPackages(const TString apkFilePaths[], int count, int timeOutInMs, bool reinstall, const TString args[], int argCount)
{
	if (GetApiLevel() < 21)
	{
		if (count == 1)
		{
			return InstallPackage(apkFilePaths[0], reinstall, args, argCount);
		}
		LogE(DEVICE, _T("Internal error : installPackages invoked with device < 21 for multiple APK"));
		return -1;
	}
	const TString mainPackageFilePath = apkFilePaths[0];
	LogDEx(DEVICE, _T("Uploading main %s and %s split APKs onto device '%s'"),
			mainPackageFilePath, count, GetSerialNumber());

	// create a installation session.
// 
// 	List<String> extraArgsList = extraArgs != null
// 		? ImmutableList.copyOf(extraArgs)
// 		: ImmutableList.<String>of();
// 
// 	String sessionId = createMultiInstallSession(apkFilePaths, extraArgsList, reinstall);
// 	if (sessionId == null) 
// 	{
// 		Log.d(mainPackageFilePath, "Failed to establish session, quit installation");
// 		throw new InstallException("Failed to establish session");
// 	}
// 	Log.d(mainPackageFilePath, String.format("Established session id=%1$s", sessionId));
// 
// 	// now upload each APK in turn.
// 	int index = 0;
// 	boolean allUploadSucceeded = true;
// 	while (allUploadSucceeded && index < apkFilePaths.size())
// 	{
// 		allUploadSucceeded = uploadAPK(sessionId, apkFilePaths.get(index), index++);
// 	}
// 
// 	// if all files were upload successfully, commit otherwise abandon the installation.
// 	String command = allUploadSucceeded
// 		? "pm install-commit " + sessionId
// 		: "pm install-abandon " + sessionId;
// 	InstallReceiver receiver = new InstallReceiver();
// 	executeShellCommand(command, receiver, timeOutInMs, TimeUnit.MILLISECONDS);
// 	String errorMessage = receiver.getErrorMessage();
// 	if (errorMessage != null)
// 	{
// 		String message = String.format("Failed to finalize session : %1$s", errorMessage);
// 		Log.e(mainPackageFilePath, message);
// 		throw new InstallException(message);
// 	}
// 	// in case not all files were upload and we abandoned the install, make sure to
// 	// notifier callers.
// 	if (!allUploadSucceeded)
// 	{
// 		throw new InstallException("Unable to upload some APKs");
// 	}
	return 0;
}

int Device::SyncPackageToDevice(const TString localFilePath, std::tstring& remotePath)
{
	const TString packageFileName = GetFileName(localFilePath);
	std::tostringstream oss;
	oss << _T("/data/local/tmp/") << packageFileName;
	remotePath = oss.str();
	const TString remoteFilePath = remotePath.c_str();

	LogDEx(DEVICE, _T("Uploading %s onto device '%s'"), packageFileName, GetSerialNumber());

	std::unique_ptr<SyncService> sync(GetSyncService());
	if (sync)
	{
		LogDEx(DEVICE, _T("Uploading file onto device '%s'"), GetSerialNumber());
		bool bSync = sync->PushFile(localFilePath, remoteFilePath, SyncService::GetNullProgressMonitor());
		sync->Close();
		if (!bSync)
		{
			return -1;
		}
	}
	return 0;
}

const TString Device::GetFileName(const TString filePath) {
	return File::GetName(filePath);
}

int Device::InstallRemotePackage(const TString remoteFilePath, bool reinstall, const TString args[], int argCount)
{
	InstallReceiver receiver;
	std::tostringstream oss;
	oss << _T("pm install");
	if (reinstall)
	{
		oss << _T(" -r");
	}
	for (int i = 0; i != argCount; i++)
	{
		oss << _T(" ") << args[i];
	}
	oss << _T(" ") << remoteFilePath;

	std::chrono::minutes minute(INSTALL_TIMEOUT_MINUTES);
	long timeout = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(minute).count());
	std::tstring& cmd = oss.str();
	ExecuteShellCommand(cmd.c_str(), &receiver, timeout);
	const TString errMsg = receiver.GetErrorMessage();
	// todo error code
	return 0;
}

int Device::RemoveRemotePackage(const TString remoteFilePath)
{
	IShellOutputReceiver& receiver = NullOutputReceiver::GetReceiver();
	std::tostringstream oss;
	oss << _T("rm \"") << remoteFilePath << _T("\"");
	std::chrono::minutes minute(INSTALL_TIMEOUT_MINUTES);
	long timeout = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(minute).count());
	return ExecuteShellCommand(oss.str().c_str(), &receiver, timeout);
}

int Device::UninstallPackage(const TString packageName)
{
	InstallReceiver receiver;
	std::tostringstream oss;
	oss << _T("pm uninstall") << packageName;
	std::chrono::minutes minute(INSTALL_TIMEOUT_MINUTES);
	long timeout = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(minute).count());
	return ExecuteShellCommand(oss.str().c_str(), &receiver, timeout);
}

void Device::SetState(DeviceState state)
{
	m_stateDev = state;
}

const TString Device::GetProperty(const TString name) const
{
	return NULL;
}

bool Device::IsOnline() const
{
	return m_stateDev == ONLINE;
}

bool Device::IsEmulator() const
{
	return false;
}

bool Device::IsOffline() const
{
	return m_stateDev == OFFLINE;
}

bool Device::IsBootLoader() const
{
	return m_stateDev == BOOTLOADER;
}

SyncService* Device::GetSyncService()
{
	SyncService* syncService = new SyncService(AndroidDebugBridge::GetSocketAddress(), this);
	if (syncService->OpenSync())
	{
		return syncService;
	}

	delete syncService;
	return NULL;
}

void Device::SetClientMonitoringSocket(SocketClient* socketClient)
{
	m_pSocketClient = socketClient;
}

SocketClient* Device::GetClientMonitoringSocket()
{
	return m_pSocketClient;
}

void Device::Update(int changeMask)
{
	m_pMonitor->GetServer()->DeviceChanged(this, changeMask);
}

//////////////////////////////////////////////////////////////////////////
// implements for InstallReceiver

void Device::InstallReceiver::ProcessNewLines(const std::vector<std::string>& vecArray)
{
	for (const std::string& line : vecArray)
	{
		if (line.empty())
		{
			continue;
		}
		if (strncmp(SUCCESS_OUTPUT, line.c_str(), sizeof(SUCCESS_OUTPUT)) == 0)
		{
			m_strErrorMessage.clear();
		}
		else
		{
			std::regex rxAdb(FAILURE_PATTERN, std::regex::icase);
			std::smatch results;
			if (std::regex_match(line, results, rxAdb) && results[0].matched)
			{
#ifdef _UNICODE
				ConvertUtils::StringToWstring(results[1].str(), m_strErrorMessage);
#else
				m_strErrorMessage = results[1].str();
#endif
			}
			else
			{
				m_strErrorMessage = _T("Unknown failure");
			}
		}
	}
}

bool Device::InstallReceiver::IsCancelled()
{
	return false;
}

const TString Device::InstallReceiver::GetErrorMessage()
{
	return m_strErrorMessage.c_str();
}
