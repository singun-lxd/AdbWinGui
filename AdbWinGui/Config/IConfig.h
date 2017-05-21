#pragma once

template <class T>
interface IConfig
{
protected:
	LPCTSTR m_szFileName;
	LPCTSTR m_szSection;

	TCHAR m_szValue[MAX_PATH] = { 0 };

public:
	IConfig(LPCTSTR lpszFileName, LPCTSTR lpszSection)
	{
		m_szFileName = lpszFileName;
		m_szSection = lpszSection;
	}

public:
	virtual T GetConfigValue() = 0;
	virtual BOOL SetConfigValue(T value) = 0;

protected:
	INT GetConfigInt(LPCTSTR lpszKeyName, int nDefault)
	{
		return ::GetPrivateProfileInt(m_szSection, lpszKeyName, nDefault, m_szFileName);
	}

	BOOL SetConfigInt(LPCTSTR lpszKeyName, int nValue)
	{
		TCHAR szValue[MAX_PATH] = { 0 };
		_itot_s(nValue, szValue, 10);
		return ::WritePrivateProfileString(m_szSection, lpszKeyName, szValue, m_szFileName);
	}

	LPCTSTR GetConfigString(LPCTSTR lpszKeyName, LPCTSTR lpszDefault)
	{
		::GetPrivateProfileString(m_szSection, lpszKeyName, lpszDefault, m_szValue, MAX_PATH, m_szFileName);
		return m_szValue;
	}

	BOOL SetConfigString(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
	{
		return ::WritePrivateProfileString(m_szSection, lpszKeyName, lpszValue, m_szFileName);
	}
};