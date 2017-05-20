#include <vcruntime.h>
#include "ConfigManager.h"

#define CONFIG_FILE_NAME _T("AdbConfig.ini")

ConfigManager* ConfigManager::s_pInstance = NULL;

ConfigManager& ConfigManager::GetInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new ConfigManager;
	}
	return *s_pInstance;
}

ConfigManager::ConfigManager() : m_configPath(CONFIG_FILE_NAME)
{

}

CString& ConfigManager::GetAdbPath()
{
	return m_configPath.GetConfigValue();
}
