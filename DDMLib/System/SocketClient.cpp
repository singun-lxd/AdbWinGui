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

#include "SocketClient.h"
#include "SocketCore.h"

SocketClient::SocketClient()
{
	m_sockClient = 0;
	m_bBlocking = true;
}

SocketClient::~SocketClient()
{
}

SocketClient* SocketClient::Open(const SocketAddress& addSocket)
{
	SocketClient* pClient = Open();
	if (pClient != NULL)
	{
		if (!pClient->Connect(addSocket))
		{
			delete pClient;
			pClient = NULL;
		}
	}
	return pClient;
}

SocketClient* SocketClient::Open()
{
	SocketClient* pClient = NULL;
	BOOL bRet = SocketCore::InitSocket();
	if (bRet)
	{
		pClient = new SocketClient();
	}
	return pClient;
}

INT SocketClient::Close()
{
	if (m_sockClient == INVALID_SOCKET)
	{
		return NO_ERROR;
	}
	INT nRet = closesocket(m_sockClient);
	if (nRet == NO_ERROR)
	{
		m_sockClient = 0;
	}
	return nRet;
}

BOOL SocketClient::IsOpen()
{
	return m_sockClient != INVALID_SOCKET;
}

BOOL SocketClient::SetTcpNoDelay(BOOL bNoDelay)
{
	INT nRet = setsockopt(m_sockClient, IPPROTO_TCP, TCP_NODELAY, (const CHAR*)&bNoDelay, sizeof(BOOL));
	if (nRet == NO_ERROR)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL SocketClient::GetTcpNoDelay()
{
	BOOL bNoDelay = FALSE;
	INT nSize = sizeof(BOOL);
	INT nRet = getsockopt(m_sockClient, IPPROTO_TCP, TCP_NODELAY, (CHAR*)&bNoDelay, &nSize);
	if (nRet == NO_ERROR)
	{
		return bNoDelay;
	}
	return FALSE;
}

BOOL SocketClient::SetTimeout(INT nTimeout)
{
	INT nRet = setsockopt(m_sockClient, SOL_SOCKET, SO_RCVTIMEO, (const CHAR*)&nTimeout, sizeof(INT));
	if (nRet == NO_ERROR)
	{
		return TRUE;
	}
	return FALSE;
}

INT SocketClient::GetTimeout()
{
	INT nTimeout = 0;
	INT nSize = sizeof(INT);
	INT nRet = getsockopt(m_sockClient, SOL_SOCKET, SO_RCVTIMEO, (CHAR *)&nTimeout, &nSize);
	if (nRet == NO_ERROR)
	{
		return nTimeout;
	}
	return -1;
}

BOOL SocketClient::ConfigureBlocking(BOOL bBlock)
{
	if (!IsOpen())
	{
		return FALSE;
	}
	if (m_bBlocking)
	{
		return TRUE;
	}
	BOOL bRet = ImplConfigureBlocking(bBlock);
	if (bRet)
	{
		m_bBlocking = bBlock;
	}
	return bRet;
}

BOOL SocketClient::Connect(const SocketAddress& addSocket)
{
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockClient == INVALID_SOCKET)
	{
		return FALSE;
	}
	INT nRet = connect(m_sockClient, (SOCKADDR*)&addSocket, sizeof(addSocket));
	if (nRet == SOCKET_ERROR)
	{
		m_sockClient = INVALID_SOCKET;
		return FALSE;
	}
	return TRUE;
}

INT SocketClient::Read(CHAR* cData, INT nLen)
{
	INT nRet = recv(m_sockClient, cData, nLen, 0);
	if (nRet == SOCKET_ERROR)
	{
		// recv error
		return -1;
	}
	return nRet;
}

INT SocketClient::Write(const CHAR* cData, INT nLen)
{
	if (nLen <= 0)
	{
		nLen = (INT) strlen(cData);
	}
	INT nRet = send(m_sockClient, cData, nLen, 0);
	if (nRet == SOCKET_ERROR)
	{
		// send error
		return -1;
	}
	return nRet;
}

BOOL SocketClient::ImplConfigureBlocking(BOOL bBlock)
{
	ULONG ulMode = static_cast<ULONG>(bBlock);
	int nRet = ioctlsocket(m_sockClient, FIONBIO, &ulMode);
	if (nRet != NO_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}
