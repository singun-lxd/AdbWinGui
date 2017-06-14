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

#include <windows.h>

class ShellHelper
{
public:
	static LPCTSTR GetErrorMessage(DWORD dwErr)
	{
		LPTSTR lpBuffer;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwErr,
			LANG_NEUTRAL,
			(LPTSTR)&lpBuffer,
			0,
			NULL);
		return const_cast<LPCTSTR>(lpBuffer);
	}

	static INT CopyFile(LPCTSTR lpszFrom, LPCTSTR lpszTo, HWND hWnd = ::GetActiveWindow())
	{
		SHFILEOPSTRUCT op;
		ZeroMemory(&op, sizeof(op));
		op.hwnd = hWnd;
		op.wFunc = FO_COPY;
		op.pFrom = lpszFrom;
		op.pTo = lpszTo;
		op.fFlags = 0;
		int nRet = ::SHFileOperation(&op);
		if (op.fAnyOperationsAborted)
		{
			nRet = -1;
		}
		return nRet;
	}
};