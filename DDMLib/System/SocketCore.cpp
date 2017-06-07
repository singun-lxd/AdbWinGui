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

#include "SocketCore.h"

#pragma comment (lib, "Ws2_32.lib")

WSADATA SocketCore::s_wsaData;
BOOL SocketCore::s_bStartUp = FALSE;

BOOL SocketCore::InitSocket()
{
	if (s_bStartUp)
	{
		return TRUE;
	}
	ZeroMemory(&s_wsaData, sizeof(s_wsaData));
	INT nRet = ::WSAStartup(MAKEWORD(2, 2), &s_wsaData);
	if (nRet == ERROR_SUCCESS)
	{
		s_bStartUp = TRUE;
		return TRUE;
	}
	return FALSE;
}

INT SocketCore::ReleaseSocket()
{
	if (!s_bStartUp)
	{
		return ERROR_SUCCESS;
	}
	INT nRet = ::WSACleanup();
	if (nRet == ERROR_SUCCESS)
	{
		ZeroMemory(&s_wsaData, sizeof(s_wsaData));
		s_bStartUp = FALSE;
	}
	return nRet;
}

INT SocketCore::GetLastError()
{
	return ::WSAGetLastError();
}

