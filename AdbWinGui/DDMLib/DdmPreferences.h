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

#include "CommonDefine.h"

class DdmPreferences
{
private:
	static int s_nTimeOut;
	static bool s_bUseAdbHost;
	static std::tstring s_strAdbHostValue;

private:
	DdmPreferences();
	static void Init();

public:
	static int GetTimeOut();
	static void SetTimeOut(int timeOut);
	static bool GetUseAdbHost();
	static void SetUseAdbHost(bool useAdbHost);
	static const TString GetAdbHostValue();
	static void SetAdbHostValue(const TString adbHostValue);
};