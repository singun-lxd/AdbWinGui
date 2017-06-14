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

#include <DbgHelp.h>
#include <Strsafe.h>

class ExceptionHandler
{
private:
	static int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
	{
		HANDLE hDumpFile = NULL;
		MINIDUMP_EXCEPTION_INFORMATION ExpParam;

		typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
			HANDLE,
			DWORD,
			HANDLE,
			MINIDUMP_TYPE,
			PMINIDUMP_EXCEPTION_INFORMATION,
			PMINIDUMP_USER_STREAM_INFORMATION,
			PMINIDUMP_CALLBACK_INFORMATION
			);

		MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
		HMODULE hDbgHelp = ::LoadLibrary(L"DbgHelp.dll");
		if (hDbgHelp)
			pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)::GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

		if (pfnMiniDumpWriteDump)
		{
			TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR szFileName[MAX_PATH] = { 0 };
			const TCHAR* szDumpName = _T("Dump");;
			TCHAR* szVersion = L"v1.0";
			SYSTEMTIME stLocalTime;

			::GetLocalTime(&stLocalTime);
			::GetModuleFileName(NULL, szPath, MAX_PATH);
			::PathRemoveFileSpec(szPath);

			::StringCchPrintf(szFileName, MAX_PATH, _T("%s\\%s"), szPath, szDumpName);
			::CreateDirectory(szFileName, NULL);

			::StringCchPrintf(szFileName, MAX_PATH, _T("%s\\%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp"),
				szPath, szDumpName, szVersion,
				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
				::GetCurrentProcessId(), ::GetCurrentThreadId());
			hDumpFile = ::CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

			::OutputDebugString(szFileName);

			if (hDumpFile != INVALID_HANDLE_VALUE)
			{
				ExpParam.ThreadId = ::GetCurrentThreadId();
				ExpParam.ExceptionPointers = pExceptionPointers;
				ExpParam.ClientPointers = FALSE;

				pfnMiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
					hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);

				::CloseHandle(hDumpFile);
			}
		}

		if (hDbgHelp != NULL)
		{
			::FreeLibrary(hDbgHelp);
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}


	static LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
	{
		if (IsDebuggerPresent())
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		return GenerateMiniDump(lpExceptionInfo);
	}

public:
	static void InitExceptionDump()
	{
		::SetUnhandledExceptionFilter(&ExceptionHandler::ExceptionFilter);
	}
};