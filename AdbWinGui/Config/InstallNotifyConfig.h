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

#include <tchar.h>
#include <wtypes.h>
#include <atlstr.h>
#include "IConfig.h"
#include "ConfigKeyDefine.h"

enum InstallNotify
{
	em_InstallDefault = -1,
	em_InstallDirect = 0,
	em_InstallWithCopy = 1,
	em_InstallNone = 2
};

class InstallNotifyConfig : public IConfig<InstallNotify>
{
private:
	InstallNotify m_emInstallNotify;

public:
	InstallNotifyConfig(LPCTSTR lpszFileName) : IConfig(lpszFileName, MAIN_SECTION)
	{
		m_emInstallNotify = em_InstallDefault;
	}

	virtual InstallNotify GetConfigValue() override
	{
		if (m_emInstallNotify != em_InstallDefault)
		{
			return m_emInstallNotify;
		}
		m_emInstallNotify = static_cast<InstallNotify>(GetConfigInt(INSTALL_CFG_KEY, em_InstallDefault));
		return m_emInstallNotify;
	}

	virtual BOOL SetConfigValue(InstallNotify value) override
	{
		int nCfgValue = static_cast<INT>(value);
		BOOL bCfgSaved = SetConfigInt(INSTALL_CFG_KEY, nCfgValue);
		if (bCfgSaved)
		{
			m_emInstallNotify = value;
		}
		return bCfgSaved;
	}
};