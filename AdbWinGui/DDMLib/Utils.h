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

class Utils
{
public:
	static void StringToWstring(const std::string& strIn, std::wstring& strOut)
	{
		std::wstringstream wss;
		wss << strIn.c_str();
		strOut = wss.str();
	}

	static void WstringToString(const std::wstring strIn, std::string& strOut)
	{
		size_t outLen = strIn.size();
		size_t converted = 0;
		setlocale(LC_CTYPE, "");
		char *pOut = new char[outLen];
		wcstombs_s(&converted, pOut, outLen, strIn.c_str(), _TRUNCATE);
		strOut = pOut;
		delete[] pOut;
	}

	template <class T>
	static std::basic_string<T>& TrimString(std::basic_string<T>& s)
	{
		if (s.empty()) {
			return s;
		}

		std::basic_string<T>::iterator c;
		for (c = s.begin(); c != s.end() && iswspace(*c++);); s.erase(s.begin(), --c);
		for (c = s.end(); c != s.begin() && iswspace(*--c);); s.erase(++c, s.end());
		return s;
	}
};