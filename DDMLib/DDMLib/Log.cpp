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

#include "Log.h"
#include <ctime>
#include "DdmPreferences.h"
#include "../System/SysLog.h"
#include "../System/ConvertUtils.h"

const TString const Log::s_arrLogLevel[LOG_LEVEL_COUT] =
{
	_T("verbose"),
	_T("debug"),
	_T("info"),
	_T("warn"),
	_T("error"),
	_T("assert"),
};

#define SPACE_LINE_LENGTH 72
Log::LogLevel Log::s_emLevel = DdmPreferences::GetLogLevel();
Log::ILogOutput* Log::s_pLogOutput = NULL;
const char* Log::s_szSpaceLine = InitSpaceLine();
const char Log::s_szHexDigit[17] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f', '\0' };

char* Log::InitSpaceLine()
{
	const int length = SPACE_LINE_LENGTH;
	char* szSpaceLine = new char[length];
	int i = length - 1;
	while (i >= 0)
		szSpaceLine[i--] = ' ';
	szSpaceLine[0] = szSpaceLine[1] = szSpaceLine[2] = szSpaceLine[3] = '0';
	szSpaceLine[4] = '-';
	return szSpaceLine;
}

Log::Log()
{
}

Log::LogLevel Log::GetLogLevelByString(const TString value)
{
	if (value == NULL)
	{
		return INVALID;
	}
	int nOffset = VERBOSE - 0;
	for (int i = 0; i < LOG_LEVEL_COUT; i++)
	{
		const TString level = s_arrLogLevel[i];
		if (_tcscmp(level, value) == 0)
		{
			return static_cast<LogLevel>(i + nOffset);
		}
	}

	return INVALID;
}

TCHAR Log::GetLogLevelPriorityLetter(LogLevel level)
{
	int nOffset = VERBOSE - 0;
	int nIndex = level - nOffset;
	const TString logName = s_arrLogLevel[nIndex];
	TCHAR chLetter = toupper(logName[0]);
	return chLetter;
}

void Log::Ex(LogLevel logLevel, const TString tag, const TString szFormat, ...)
{
	if (IsNeedLog(logLevel))
	{
		const int LOG_BUFFER_COUNT = 512;
		va_list args;
		va_start(args, szFormat);
		int nBuf;
		TCHAR szBuffer[LOG_BUFFER_COUNT] = { 0 };
		nBuf = _vsntprintf(szBuffer, LOG_BUFFER_COUNT - 1, szFormat, args);
		Println(logLevel, tag, szBuffer);
		va_end(args);
	}
}

void Log::V(const TString tag, const TString message)
{
	Println(LogLevel::VERBOSE, tag, message);
}

void Log::D(const TString tag, const TString message)
{
	Println(LogLevel::DEBUG, tag, message);
}

void Log::I(const TString tag, const TString message)
{
	Println(LogLevel::INFO, tag, message);
}

void Log::W(const TString tag, const TString message)
{
	Println(LogLevel::WARN, tag, message);
}

void Log::E(const TString tag, const TString message)
{
	Println(LogLevel::ERROR, tag, message);
}

void Log::LogAndDisplay(LogLevel logLevel, const TString tag, const TString message)
{
	if (s_pLogOutput != NULL)
	{
		s_pLogOutput->PrintAndPromptLog(logLevel, tag, message);
	}
	else
	{
		Println(logLevel, tag, message);
	}
}

void Log::SetLevel(LogLevel logLevel)
{
	s_emLevel = logLevel;
}

void Log::SetLogOutput(ILogOutput* logOutput)
{
	s_pLogOutput = logOutput;
}

void Log::HexDump(const TString tag, LogLevel level, char data[], int offset, int length)
{
	int kHexOffset = 6;
	int kAscOffset = 55;
	char line[SPACE_LINE_LENGTH] = { 0 };
	int addr, baseAddr, count;
	int i, ch;
	bool needErase = true;

	//Log.w(tag, "HEX DUMP: off=" + offset + ", length=" + length);

	baseAddr = 0;
	while (length != 0)
	{
		if (length > 16)
		{
			// full line
			count = 16;
		}
		else
		{
			// partial line; re-copy blanks to clear end
			count = length;
			needErase = true;
		}

		if (needErase)
		{
			strncpy(line, s_szSpaceLine, SPACE_LINE_LENGTH);
			needErase = false;
		}

		// output the address (currently limited to 4 hex digits)
		addr = baseAddr;
		addr &= 0xffff;
		ch = 3;
		unsigned int uAddr;
		while (addr != 0)
		{
			line[ch] = s_szHexDigit[addr & 0x0f];
			ch--;
			uAddr = (unsigned int) addr;
			uAddr >>= 4;
			addr = (int) uAddr;
		}

		// output hex digits and ASCII chars
		ch = kHexOffset;
		unsigned char uVal;
		for (i = 0; i < count; i++)
		{
			char val = data[offset + i];

			uVal = (unsigned char) val;
			uVal >>= 4;
			line[ch++] = s_szHexDigit[((char) uVal) & 0x0f];
			line[ch++] = s_szHexDigit[val & 0x0f];
			ch++;

			if (val >= 0x20 && val < 0x7f)
				line[kAscOffset + i] = (char)val;
			else
				line[kAscOffset + i] = '.';
		}

		const TString outLine;
#ifdef _UNICODE
		std::tstring lineOut;
		ConvertUtils::StringToWstring(line, lineOut);
		outLine = lineOut.c_str();
#else	
		outLine = line;
#endif
		Println(level, tag, outLine);

		// advance to next chunk of data
		length -= count;
		offset += count;
		baseAddr += count;
	}
}

void Log::HexDump(char data[], int length)
{
	HexDump(_T("ddms"), LogLevel::DEBUG, data, 0, length);
}

void Log::PrintLog(LogLevel logLevel, const TString tag, const TString message)
{
	time_t currenttime = time(0);
	TCHAR szTime[20] = { 0 };
	struct tm timeRet;
	localtime_s(&timeRet, &currenttime);
	_tcsftime(szTime, MAX_PATH, _T("%Y-%m-%d %H:%M:%S"), &timeRet);

	TCHAR chLetter = GetLogLevelPriorityLetter(logLevel);

	// "%s %c/[%s]: %s\n"
	std::tostringstream oss;
	oss << szTime << _T(" ") << chLetter << _T("/");
	oss << _T("[") << tag << _T("]: ") << message << std::endl;
	std::tstring& ostr = oss.str();
	SysLog::Log(ostr.c_str());
}

void Log::Println(LogLevel logLevel, const TString tag, const TString message)
{
	if (IsNeedLog(logLevel))
	{
		if (s_pLogOutput != NULL)
		{
			s_pLogOutput->PrintLog(logLevel, tag, message);
		}
		else
		{
			PrintLog(logLevel, tag, message);
		}
	}
}

bool Log::IsNeedLog(LogLevel logLevel)
{
	if (logLevel == LogLevel::INVALID)
	{
		return false;
	}
	return logLevel >= s_emLevel;
}
