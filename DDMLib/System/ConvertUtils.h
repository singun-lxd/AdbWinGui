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

#include "SysDef.h"

class ConvertUtils
{
public:
	static void StringToWstring(const std::string& strIn, std::wstring& strOut)
	{
		StringToWstring(strIn.c_str(), strOut);
	}

	static void StringToWstring(const CHAR* szIn, std::wstring& strOut)
	{
		if (szIn == NULL)
		{
			return;
		}
		std::wstringstream wss;
		wss << szIn;
		strOut = wss.str();
	}

	static void WstringToString(const std::wstring strIn, std::string& strOut)
	{
		WstringToString(strIn.c_str(), strOut);
	}

	static void WstringToString(const WCHAR* szIn, std::string& strOut)
	{
		if (szIn == NULL)
		{
			return;
		}
		size_t outLen = _tcslen(szIn);
		size_t converted = 0;
		setlocale(LC_CTYPE, "");
		char *pOut = new char[outLen];
		wcstombs_s(&converted, pOut, outLen, szIn, _TRUNCATE);
		strOut = pOut;
		delete[] pOut;
	}

};