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

#include "../CommonDefine.h"
#include "ConvertUtils.h"

struct SocketAddress : public SOCKADDR_IN
{
public:
	SocketAddress()
	{
		sin_family = AF_INET;
	}

	SocketAddress(const TString addr, unsigned short uPort)
	{
		sin_family = AF_INET;
		SetSocketAddress(addr);
		SetSocketPort(uPort);
	}

	void SetSocketAddress(const TString addr)
	{
		sin_addr.s_addr = InetPton(sin_family, addr, this);
	}

	void SetSocketPort(unsigned short uPort)
	{
		sin_port = htons(uPort);
	}
};