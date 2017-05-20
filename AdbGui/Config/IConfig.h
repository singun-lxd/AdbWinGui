#pragma once

#define APP_NAME _T("AdbGui")

template <class T>
interface IConfig
{
protected:
	LPCTSTR m_szFileName;

	TCHAR m_szValue[MAX_PATH] = { 0 };

public:
	IConfig(LPCTSTR lpszFileName)
	{
		m_szFileName = lpszFileName;
	}

public:
	virtual T GetConfigValue() = 0;

protected:
	INT GetConfigInt(LPCTSTR lpszKeyName, int nDefault)
	{
		return ::GetPrivateProfileInt(APP_NAME, lpszKeyName, nDefault, m_szFileName);
	}

	BOOL SetConfigInt(LPCTSTR lpszKeyName, int nValue)
	{
		TCHAR szValue[MAX_PATH] = { 0 };
		_itot_s(nValue, szValue, 10);
		return ::WritePrivateProfileString(APP_NAME, lpszKeyName, szValue, m_szFileName);
	}

	LPCTSTR GetConfigString(LPCTSTR lpszKeyName, LPCTSTR lpszDefault)
	{
		::GetPrivateProfileString(APP_NAME, lpszKeyName, lpszDefault, m_szValue, MAX_PATH, m_szFileName);
		return m_szValue;
	}

	BOOL SetConfigString(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
	{
		return ::WritePrivateProfileString(APP_NAME, lpszKeyName, lpszValue, m_szFileName);
	}
};