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

#include <wtypes.h>
#include <tchar.h>

#define ENV_BUFFER_SIZE		4096

#define ANDROID_ENV			_T("ANDROID_HOME")
#define PATH_ENV				_T("PATH")
#define SERVER_PORT_ENV		_T("ANDROID_ADB_SERVER_PORT")

class AndroidEnvVar
{
private:
	TCHAR m_szBuffer[ENV_BUFFER_SIZE] = { 0 };

public:
	LPCTSTR GetAndroidHome()
	{
		DWORD dwRet = ::GetEnvironmentVariable(ANDROID_ENV, m_szBuffer, MAX_PATH);
		if (dwRet > 0)
		{
			return m_szBuffer;
		}
		return NULL;
	}

	BOOL SetAndroidHome(LPCTSTR lpszEnvValue)
	{
		return ::SetEnvironmentVariable(ANDROID_ENV, lpszEnvValue);
	}

	LPTSTR GetPathValue()
	{
		DWORD dwRet = ::GetEnvironmentVariable(PATH_ENV, m_szBuffer, ENV_BUFFER_SIZE);
		if (dwRet > 0)
		{
			return m_szBuffer;
		}
		return NULL;
	}

	INT GetAdbServerPort()
	{
		int nRet = -1;
		DWORD dwRet = ::GetEnvironmentVariable(SERVER_PORT_ENV, m_szBuffer, MAX_PATH);
		if (dwRet > 0)
		{
			nRet = _ttoi(m_szBuffer);
		}
		return nRet;
	}
};