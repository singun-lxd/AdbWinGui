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
#include "SocketAddress.h"

class SocketClient
{
private:
	SOCKET m_sockClient;
	BOOL m_bBlocking;

private:
	SocketClient();
public:
	~SocketClient();
	static SocketClient* Open();
	static SocketClient* Open(const SocketAddress& addSocket);
	INT Close();
	BOOL IsOpen();
	BOOL SetTcpNoDelay(BOOL bNoDelay);
	BOOL GetTcpNoDelay();
	BOOL ConfigureBlocking(BOOL bBlock);
	BOOL Connect(const SocketAddress& addSocket);
	INT Read(CHAR* cData, INT nLen);
	INT Write(const CHAR* cData, INT nLen = -1);

private:
	BOOL ImplConfigureBlocking(BOOL bBlock);
};