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

#define ENV_BUFFER_SIZE			4096

#define SERVER_PORT_ENV			_T("ANDROID_ADB_SERVER_PORT")
#define INSTALL_TIMEOUT_ENV		_T("ADB_INSTALL_TIMEOUT")

class AndroidEnvVar
{
private:
	TCHAR m_szBuffer[ENV_BUFFER_SIZE] = { 0 };

protected:
	LPTSTR GetResultString(LPCTSTR lpszVarName)
	{
		size_t requiredSize = 0;
		_tgetenv_s(&requiredSize, NULL, 0, lpszVarName);
		if (requiredSize == 0 || requiredSize > ENV_BUFFER_SIZE)
		{
			return NULL;
		}
		_tgetenv_s(&requiredSize, m_szBuffer, requiredSize, lpszVarName);
		return m_szBuffer;
	}

	inline LPCTSTR GetString(LPCTSTR lpszVarName)
	{
		return GetResultString(lpszVarName);
	}

	INT GetInteger(LPCTSTR lpszVarName)
	{
		LPCTSTR lpszValue = GetString(lpszVarName);
		if (lpszValue != NULL)
		{
			return _ttoi(lpszValue);
		}
		return -1;
	}

	LONG GetLong(LPCTSTR lpszVarName)
	{
		LPCTSTR lpszValue = GetString(lpszVarName);
		if (lpszValue != NULL)
		{
			return _ttol(lpszValue);
		}
		return -1L;
	}

	BOOL SetString(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
	{
		int nRet = _tputenv_s(lpszVarName, lpszVarValue);
		if (nRet == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

public:
	INT GetAdbServerPort()
	{
		return GetInteger(SERVER_PORT_ENV);
	}

	LONG GetInstallTimeOut()
	{
		return GetLong(INSTALL_TIMEOUT_ENV);
	}
};