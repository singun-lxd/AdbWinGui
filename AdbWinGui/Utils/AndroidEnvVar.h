#pragma once

#include <wtypes.h>
#include <tchar.h>

#define ENV_BUFFER_SIZE	4096

#define ANDROID_ENV		_T("ANDROID_HOME")
#define PATH_ENV			_T("PATH")

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
};