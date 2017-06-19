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
#include <locale>
#include <xfunctional>
#include <algorithm>

class StringUtils
{
public:
	template <class T>
	inline static void TrimString(std::basic_string<T> &s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { std::locale loc; return std::isspace(c, loc); });
		auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {	std::locale loc; return std::isspace(c, loc); }).base();
		if (wsback <= wsfront)
		{
			s.clear();
		}
		else
		{
			s.erase(wsback, s.end());
			s.erase(s.begin(), wsfront);
		}
	}

	template <class T>
	static std::basic_string<T>& ToLowerCase(std::basic_string<T>& s)
	{
		if (s.empty())
		{
			return s;
		}
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}

	template <class T>
	static std::basic_string<T>& ToUpperCase(std::tstring& strIn)
	{
		if (s.empty())
		{
			return s;
		}
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	static const char* SaveString(const char* value)
	{
		return value;
	}

#define EnumToString(val) StringUtils::SaveString(#val)  
};
