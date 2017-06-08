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

namespace Log
{
#define LOG_BUFFER_COUNT  512

	inline void LogEx(const TString lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		int nBuf;
		TCHAR szBuffer[LOG_BUFFER_COUNT] = { 0 };
		nBuf = _vsntprintf_s(szBuffer, LOG_BUFFER_COUNT - 1, lpszFormat, args);
		::OutputDebugString(szBuffer);
		::OutputDebugString(_T("\r\n"));
		va_end(args);
	}
};

#define LogLEx(content, ...)		Log::LogEx(content, ##__VA_ARGS__)
#define LogL(content)				::OutputDebugString(content);::OutputDebugString(_T("\r\n"))

#ifdef _DEBUG
#define		LogV(content)			LogL(content)
#define		LogD(content) 			LogL(content)
#define		LogI(content) 			LogL(content)
#define		LogW(content) 			LogL(content)
#define		LogE(content) 			LogL(content)
#define		LogVEx(content, ...)	LogLEx(content, ##__VA_ARGS__)
#define		LogDEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#define		LogIEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#define		LogWEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#define		LogEEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#else
#define		LogV(content)
#define		LogD(content)
#define		LogI(content) 			LogL(content)
#define		LogW(content) 			LogL(content)
#define		LogE(content) 			LogL(content)
#define		LogVEx(content)
#define		LogDEx(content)
#define		LogIEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#define		LogWEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#define		LogEEx(content, ...) 	LogLEx(content, ##__VA_ARGS__)
#endif