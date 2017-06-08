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

#include "DdmPreferences.h"

#ifdef _DEBUG
#define DEFAULT_LOG_LEVEL			Log::LogLevel::VERBOSE
#else
#define DEFAULT_LOG_LEVEL			Log::LogLevel::ERROR
#endif
#define DEFAULT_TIMEOUT			5000 // standard delay, in ms
#define DEFAULT_USE_ADBHOST		false;
#define DEFAULT_ADBHOST_VALUE		_T("127.0.0.1");

Log::LogLevel DdmPreferences::s_emLogLevel = DEFAULT_LOG_LEVEL;
int DdmPreferences::s_nTimeOut = DEFAULT_TIMEOUT;
bool DdmPreferences::s_bUseAdbHost = DEFAULT_USE_ADBHOST;
std::tstring DdmPreferences::s_strAdbHostValue = DEFAULT_ADBHOST_VALUE;

DdmPreferences::DdmPreferences()
{
}

Log::LogLevel DdmPreferences::GetLogLevel()
{
	return s_emLogLevel;
}

void DdmPreferences::SetLogLevel(const TString value)
{
	s_emLogLevel = Log::GetLogLevelByString(value);

	Log::SetLevel(s_emLogLevel);
}

int DdmPreferences::GetTimeOut()
{
	return s_nTimeOut;
}

void DdmPreferences::SetTimeOut(int timeOut)
{
	s_nTimeOut = timeOut;
}

bool DdmPreferences::GetUseAdbHost()
{
	return s_bUseAdbHost;
}

void DdmPreferences::SetUseAdbHost(bool useAdbHost)
{
	s_bUseAdbHost = useAdbHost;
}

const TString DdmPreferences::GetAdbHostValue() {
	return s_strAdbHostValue.c_str();
}

void DdmPreferences::SetAdbHostValue(const TString adbHostValue) {
	s_strAdbHostValue = adbHostValue;
}

