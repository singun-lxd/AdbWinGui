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

#include "AdbPathConfig.h"
#include "ApkDirectoryConfig.h"
#include "InstallNotifyConfig.h"
#include "ReplaceNotifyConfig.h"

class ConfigManager
{
private:
	static TCHAR s_szPath[MAX_PATH];
	static ConfigManager* s_pInstance;
private:
	AdbPathConfig m_configPath;
	ApkDirectoryConfig m_configDirectory;
	InstallNotifyConfig m_configInstallNotify;
	ReplaceNotifyConfig m_configReplaceNotify;

public:
	static ConfigManager& GetInstance();

public:
	ConfigManager(LPCTSTR lpszFileName);
	AdbPathConfig::PathMode GetAdbPathMode();
	const CString& GetAdbPath();
	const CString& AutoAdbPath();
	void SetAdbPath(const CString& strPath);
	ApkDirectoryConfig::DirectoryMode GetApkDirMode();
	const CString& GetApkDir();
	const CString& CurrentApkDir();
	void SetApkDir(const CString& strPath);
	InstallNotify GetInstallNotifyConfig();
	void SetInstallNotifyConfig(InstallNotify notify);
	BOOL GetForceReplace();
	void SetForceReplace(BOOL bForce);
};