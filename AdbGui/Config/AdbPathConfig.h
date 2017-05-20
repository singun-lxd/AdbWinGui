#pragma once

#include <tchar.h>
#include <wtypes.h>
#include <atlstr.h>
#include "IConfig.h"
#include "..\Utils\AdbPath.h"

#define ADB_PATH_KEY _T("ADB_PATH")

class AdbPathConfig : public IConfig<CString&>
{
private:
	CString m_strAdbPath;
	AdbPath* m_pAdbPath;

public:
	AdbPathConfig(LPCTSTR lpszFileName) : IConfig(lpszFileName)
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
	CString& GetConfigValue()
	{
		if (!m_strAdbPath.IsEmpty())
		{
			return m_strAdbPath;
		}
		LPCTSTR lpszPath = GetConfigString(ADB_PATH_KEY, _T(""));
		if (lpszPath != NULL && _tcslen(lpszPath) > 0)
		{
			m_strAdbPath = lpszPath;
		}
		else
		{
			m_pAdbPath = new AdbPath;
			lpszPath = m_pAdbPath->GetAdbExePath();
			if (lpszPath != NULL && _tcslen(lpszPath) > 0)
			{
				m_strAdbPath = lpszPath;
			}
		}
		return m_strAdbPath;
	}
};