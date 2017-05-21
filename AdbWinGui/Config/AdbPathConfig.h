#pragma once

#include <tchar.h>
#include <wtypes.h>
#include <atlstr.h>
#include "IConfig.h"
#include "ConfigKeyDefine.h"
#include "..\Utils\AdbPath.h"

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
	const CString& GetConfigValue()
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

	BOOL SetConfigValue(const CString& value)
	{
		BOOL bRet = FALSE;
		if (value.IsEmpty())
		{
			m_emPathMode = em_PathModeAuto;
			BOOL modeSaved = SetConfigInt(PATH_MODE_KEY, m_emPathMode);
			BOOL pathSaved = SetConfigString(ADB_PATH_KEY, _T(""));
			DWORD dwRet = ::GetLastError();
			bRet = modeSaved && pathSaved;
		}
		else
		{
			m_emPathMode = em_PathModeManual;
			BOOL modeSaved = SetConfigInt(PATH_MODE_KEY, m_emPathMode);
			BOOL pathSaved = SetConfigString(ADB_PATH_KEY, value);
			DWORD dwRet = ::GetLastError();
			bRet = modeSaved && pathSaved;
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