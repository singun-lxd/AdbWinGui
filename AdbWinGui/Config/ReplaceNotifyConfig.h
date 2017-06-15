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

class ReplaceNotifyConfig : public IConfig<BOOL>
{
private:
	INT m_nReplace;

public:
	ReplaceNotifyConfig(LPCTSTR lpszFileName) : IConfig(lpszFileName, MAIN_SECTION)
	{
		m_nReplace = -1;
	}

	virtual BOOL GetConfigValue() override
	{
		if (m_nReplace != -1)
		{
			return m_nReplace;
		}
		m_nReplace = GetConfigInt(FORCE_REPLACE_KEY, FALSE);
		return static_cast<BOOL>(m_nReplace);
	}

	virtual BOOL SetConfigValue(BOOL value) override
	{
		INT nCfgValue = static_cast<INT>(value);
		BOOL bCfgSaved = SetConfigInt(FORCE_REPLACE_KEY, nCfgValue);
		if (bCfgSaved)
		{
			m_nReplace = static_cast<INT>(value);
		}
		return bCfgSaved;
	}
};