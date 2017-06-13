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

class ApkDirectoryConfig : public IConfig<const CString&>
{
public:
	enum DirectoryMode
	{
		em_DirModeCurrent = 0,
		em_DirModeCustom = 1,
	};

private:
	DirectoryMode m_emDirMode;
	CString m_strApkDir;

public:
	ApkDirectoryConfig(LPCTSTR lpszDirectoryName) : IConfig(lpszDirectoryName, MAIN_SECTION)
	{
	}

	~ApkDirectoryConfig()
	{
	}

public:
	virtual const CString & GetConfigValue() override
	{
		if (!m_strApkDir.IsEmpty())
		{
			return m_strApkDir;
		}
		m_emDirMode = static_cast<DirectoryMode>(GetConfigInt(DIR_MODE_KEY, em_DirModeCurrent));

		if (m_emDirMode == em_DirModeCurrent)
		{
			SetCurrentApkDir();
		}
		else
		{
			LPCTSTR lpszPath = GetConfigString(APK_DIR_KEY, _T(""));
			if (lpszPath != NULL && _tcslen(lpszPath) > 0)
			{
				m_strApkDir = lpszPath;
			}
		}
		return m_strApkDir;
	}

	void SetCurrentApkDir()
	{
		LPTSTR lpszApkDir = m_strApkDir.GetBuffer(MAX_PATH);
		::GetModuleFileName(NULL, lpszApkDir, MAX_PATH);
		::PathRemoveFileSpec(lpszApkDir);
		m_strApkDir.ReleaseBuffer();
	}

	virtual BOOL SetConfigValue(const CString& value) override
	{
		BOOL bRet = FALSE;
		if (value.IsEmpty())
		{
			m_emDirMode = em_DirModeCurrent;
			BOOL modeSaved = SetConfigInt(DIR_MODE_KEY, m_emDirMode);
			BOOL dirSaved = SetConfigString(APK_DIR_KEY, _T(""));
			bRet = modeSaved && dirSaved;
		}
		else
		{
			m_emDirMode = em_DirModeCustom;
			BOOL modeSaved = SetConfigInt(DIR_MODE_KEY, m_emDirMode);
			BOOL dirSaved = SetConfigString(APK_DIR_KEY, value);
			bRet = modeSaved && dirSaved;
		}
		if (bRet)
		{
			m_strApkDir = value;
		}
		return bRet;
	}

	const CString& CurrentApkDir()
	{
		m_strApkDir.Empty();
		SetCurrentApkDir();
		if (!m_strApkDir.IsEmpty())
		{
			m_emDirMode = em_DirModeCurrent;
		}
		return m_strApkDir;
	}

	DirectoryMode GetApkDirMode()
	{
		return m_emDirMode;
	}
};
