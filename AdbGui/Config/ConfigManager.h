#pragma once

#include "AdbPathConfig.h"

class ConfigManager
{
private:
	static ConfigManager* s_pInstance;
private:
	AdbPathConfig m_configPath;

public:
	static ConfigManager& GetInstance();

public:
	ConfigManager();
	CString& GetAdbPath();
};