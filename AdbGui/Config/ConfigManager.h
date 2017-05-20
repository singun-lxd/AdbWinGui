#pragma once

#include "AdbPathConfig.h"

class ConfigManager
{
private:
	static TCHAR s_szPath[MAX_PATH];
	static ConfigManager* s_pInstance;
private:
	AdbPathConfig m_configPath;

public:
	static ConfigManager& GetInstance();

public:
	ConfigManager(LPCTSTR lpszFileName);
	AdbPathConfig::PathMode GetAdbPathMode();
	const CString& GetAdbPath();
	const CString& UpdateAdbPath();
	void SetAdbPath(const CString& strPath);
};