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
#include "../Utils/AdbPath.h"

class AdbPathConfig : public IConfig<const CString&>
{
public:
	enum PathMode
	{
		em_PathModeAuto = 0,
		em_PathModeManual = 1,
	};

private:
	PathMode m_emPathMode;
	CString m_strAdbPath;
	AdbPath* m_pAdbPath;

public:
	AdbPathConfig(LPCTSTR lpszFileName) : IConfig(lpszFileName, MAIN_SECTION)
	{
		m_pAdbPath = NULL;
	}

	~AdbPathConfig()
	{
		if (m_pAdbPath != NULL)
		{
			delete m_pAdbPath;
			m_pAdbPath = NULL;
		}
	}

public:
	virtual const CString& GetConfigValue() override
	{
		if (!m_strAdbPath.IsEmpty())
		{
			return m_strAdbPath;
		}
		m_emPathMode = static_cast<PathMode>(GetConfigInt(PATH_MODE_KEY, em_PathModeAuto));

		if (m_emPathMode == em_PathModeAuto)
		{
			AutoUpdateAdbPath();
		}
		else
		{
			LPCTSTR lpszPath = GetConfigString(ADB_PATH_KEY, _T(""));
			if (lpszPath != NULL && _tcslen(lpszPath) > 0)
			{
				m_strAdbPath = lpszPath;
			}
		}
		return m_strAdbPath;
	}

	void AutoUpdateAdbPath()
	{
		if (m_pAdbPath == NULL)
		{
			m_pAdbPath = new AdbPath;
		}
		LPCTSTR lpszPath = m_pAdbPath->GetAdbExePath();
		if (lpszPath != NULL && _tcslen(lpszPath) > 0)
		{
			m_strAdbPath = lpszPath;
		}
	}

	virtual BOOL SetConfigValue(const CString& value) override
	{
		BOOL bRet = FALSE;
		if (value.IsEmpty())
		{
			m_emPathMode = em_PathModeAuto;
			BOOL modeSaved = SetConfigInt(PATH_MODE_KEY, m_emPathMode);
			BOOL pathSaved = SetConfigString(ADB_PATH_KEY, _T(""));
			bRet = modeSaved && pathSaved;
		}
		else
		{
			m_emPathMode = em_PathModeManual;
			BOOL modeSaved = SetConfigInt(PATH_MODE_KEY, m_emPathMode);
			BOOL pathSaved = SetConfigString(ADB_PATH_KEY, value);
			bRet = modeSaved && pathSaved;
		}
		if (bRet)
		{
			m_strAdbPath = value;
		}
		return bRet;
	}

	const CString& AutoAdbPath()
	{
		m_strAdbPath.Empty();
		AutoUpdateAdbPath();
		if (!m_strAdbPath.IsEmpty())
		{
			m_emPathMode = em_PathModeAuto;
		}
		return m_strAdbPath;
	}

	PathMode GetAdbPathMode()
	{
		return m_emPathMode;
	}
};