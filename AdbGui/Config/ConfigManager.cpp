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

const CString& ConfigManager::UpdateAdbPath()
{
	return m_configPath.UpdateAdbPath();
}

void ConfigManager::SetAdbPath(const CString& strPath)
{
	m_configPath.SetConfigValue(strPath);
}
