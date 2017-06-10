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

#define LOG_LEVEL_COUT  6

// fix redefine error
#ifdef ERROR
#undef ERROR
#endif

class Log
{
public:
	static const TString const s_arrLogLevel[LOG_LEVEL_COUT];
	enum LogLevel
	{
		INVALID = 1,
		VERBOSE = 2,
		DEBUG = 3,
		INFO = 4,
		WARN = 5,
		ERROR = 6,
		ASSERT = 7,
	};

	interface ILogOutput
	{
		virtual void PrintLog(LogLevel logLevel, const TString tag, const TString message) = 0;
		virtual void PrintAndPromptLog(LogLevel logLevel, const TString tag, const TString message) = 0;
	};

private:
	static LogLevel s_emLevel;
	static ILogOutput* s_pLogOutput;
	static const char* s_szSpaceLine;
	static const char s_szHexDigit[17];

	static char* InitSpaceLine();

private:
	Log();

public:
	static LogLevel GetLogLevelByString(const TString value);
	static TCHAR GetLogLevelPriorityLetter(LogLevel level);

	static void Ex(LogLevel logLevel, const TString tag, const TString szFormat, ...);
	static void V(const TString tag, const TString message);
	static void D(const TString tag, const TString message);
	static void I(const TString tag, const TString message);
	static void W(const TString tag, const TString message);
	static void E(const TString tag, const TString message);
	static void LogAndDisplay(LogLevel logLevel, const TString tag, const TString message);

	static void SetLevel(LogLevel logLevel);
	static void SetLogOutput(ILogOutput* logOutput);
	static void HexDump(const TString tag, LogLevel level, char data[], int offset, int length);
	static void HexDump(char data[], int length);
	static void PrintLog(LogLevel logLevel, const TString tag, const TString message);

private:
	static void Println(LogLevel logLevel, const TString tag, const TString message);
	static inline bool IsNeedLog(LogLevel logLevel);
};

#define LogEx(...)						Log::Ex(##__VA_ARGS__)	

#ifdef _DEBUG
#define		LogV(tag, msg)				Log::V(tag, msg)
#define		LogD(tag, msg) 			Log::D(tag, msg)
#define		LogI(tag, msg) 			Log::I(tag, msg)
#define		LogW(tag, msg) 			Log::W(tag, msg)
#define		LogE(tag, msg) 			Log::E(tag, msg)
#define		LogVEx(tag, fmt, ...)		LogEx(Log::LogLevel::VERBOSE, tag, fmt, ##__VA_ARGS__)
#define		LogDEx(tag, fmt, ...) 		LogEx(Log::LogLevel::DEBUG, tag, fmt, ##__VA_ARGS__)
#define		LogIEx(tag, fmt, ...) 		LogEx(Log::LogLevel::INFO, tag, fmt, ##__VA_ARGS__)
#define		LogWEx(tag, fmt, ...) 		LogEx(Log::LogLevel::WARN, tag, fmt, ##__VA_ARGS__)
#define		LogEEx(tag, fmt, ...) 		LogEx(Log::LogLevel::ERROR, tag, fmt, ##__VA_ARGS__)
#else
#define		LogV(tag, msg)
#define		LogD(tag, msg)
#define		LogI(tag, msg) 			Log::I(tag, msg)
#define		LogW(tag, msg) 			Log::W(tag, msg)
#define		LogE(tag, msg) 			Log::E(tag, msg)
#define		LogVEx(tag, fmt, ...)
#define		LogDEx(tag, fmt, ...)
#define		LogIEx(tag, fmt, ...) 		LogEx(Log::LogLevel::INFO, tag, fmt, ##__VA_ARGS__)
#define		LogWEx(tag, fmt, ...) 		LogEx(Log::LogLevel::WARN, tag, fmt, ##__VA_ARGS__)
#define		LogEEx(tag, fmt, ...) 		LogEx(Log::LogLevel::ERROR, tag, fmt, ##__VA_ARGS__)
#endif