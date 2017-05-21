#pragma once

#include <shlwapi.h>
#include "AndroidEnvVar.h"

#define ADB_PATH		_T("platform-tools")
#define ADB_EXE		_T("adb.exe")

class AdbPath : public AndroidEnvVar
{
private:
	TCHAR m_szAdbExePath[MAX_PATH] = { 0 };

public:
	LPCTSTR GetAdbExePath()
	{
		if (CheckAdbExists())
		{
			return m_szAdbExePath;
		}
		else
		{
			LPCTSTR lpszAdbPath = GetAndroidHome();
			if (lpszAdbPath != NULL && _tcslen(lpszAdbPath) > 0)
			{
				_tcscpy_s(m_szAdbExePath, lpszAdbPath);
				::PathAppend(m_szAdbExePath, ADB_PATH);
				::PathAppend(m_szAdbExePath, ADB_EXE);
				if (::PathFileExists(m_szAdbExePath))
				{
					return m_szAdbExePath;
				}
			}
			return NULL;
		}
	}

	BOOL CheckAdbExists()
	{
		if (::PathFileExists(ADB_EXE))
		{
			_tcscpy_s(m_szAdbExePath, ADB_EXE);
			return TRUE;
		}
		LPTSTR szPath = GetPathValue();
		if (szPath != NULL && _tcslen(szPath) > 0)
		{
			TCHAR* szContext = NULL;
			TCHAR* szToken = _tcstok_s(szPath, _T(";"), &szContext);
			while (szToken != NULL)
			{
				if (_tcslen(szToken) > 0)
				{
					_tcscpy_s(m_szAdbExePath, szToken);
					::PathAppend(m_szAdbExePath, ADB_EXE);
					if (::PathFileExists(m_szAdbExePath))
					{
						return TRUE;
					}
				}
				szToken = _tcstok_s(NULL, _T(";"), &szContext);
			}
		}
		return FALSE;
	}
};