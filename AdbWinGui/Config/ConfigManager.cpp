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

#include <vcruntime.h>
#include "ConfigManager.h"

#define CONFIG_FILE_NAME _T("AdbConfig.ini")

TCHAR ConfigManager::s_szPath[MAX_PATH] = { 0 };
ConfigManager* ConfigManager::s_pInstance = NULL;

ConfigManager& ConfigManager::GetInstance()
{
	if (s_pInstance == NULL)
	{
		::GetModuleFileName(NULL, s_szPath, MAX_PATH);
		::PathRemoveFileSpec(s_szPath);
		::PathAppend(s_szPath, CONFIG_FILE_NAME);
		s_pInstance = new ConfigManager(s_szPath);
	}
	return *s_pInstance;
}

ConfigManager::ConfigManager(LPCTSTR lpszFileName) : m_configPath(lpszFileName)
{
}

AdbPathConfig::PathMode ConfigManager::GetAdbPathMode()
{
	return m_configPath.GetAdbPathMode();
}

const CString& ConfigManager::GetAdbPath()
{
	return m_configPath.GetConfigValue();
}

const CString& ConfigManager::AutoAdbPath()
{
	return m_configPath.AutoAdbPath();
}

void ConfigManager::SetAdbPath(const CString& strPath)
{
	m_configPath.SetConfigValue(strPath);
}
