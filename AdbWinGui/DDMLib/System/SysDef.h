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

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <wtypes.h>
#include <tchar.h>

namespace std
{

#ifdef _UNICODE
#define		TString				WCHAR*
#define		tstring				wstring
#define		tstringstream		wstringstream
#define		tistringstream		wistringstream
#define		tostringstream		wostringstream
#define		tregex				wregex
#define		tsmatch				wsmatch
#else
#define		TString				CHAR*
#define		tstring				string
#define		tstringstream		stringstream
#define		tistringstream		istringstream
#define		tostringstream		ostringstream
#define		tregex				regex
#define		tsmatch				std::smatch
#endif

}

#ifndef _countof
	#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

