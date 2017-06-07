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

#include "AdbHelper.h"
#include <thread>
#include "DdmPreferences.h"
#include "../System/SocketCore.h"

#define WAIT_TIME		5 // spin-wait sleep, in ms

AdbHelper::AdbHelper()
{
}

int AdbHelper::GetLastError()
{
	return SocketCore::GetLastError();
}

int AdbHelper::ReleaseSocket()
{
	return SocketCore::ReleaseSocket();
}

const char* AdbHelper::FormAdbRequest(const char* req)
{
	std::ostringstream ss;
	ss.fill(_T('0'));
	ss.width(4);
	ss << std::hex << strlen(req);
	ss << req;

	std::string& resultStr = ss.str();
	size_t size = resultStr.length() + 1;
	char* result = new char[size];
	strcpy_s(result, size, resultStr.c_str());

	return result;
}

AdbHelper::AdbResponse* AdbHelper::ReadAdbResponse(SocketClient* client, bool readDiagString)
{
	AdbResponse* pResp = new AdbResponse();

	char reply[5] = { 0 };
	Read(client, reply, 4);

	if (IsOkay(reply))
	{
		pResp->okay = true;
	}
	else
	{
		readDiagString = true; // look for a reason after the FAIL
		pResp->okay = false;
	}

	if (readDiagString)
	{
		// length string is in next 4 bytes
		char lenBuf[5] = { 0 };
		Read(client, lenBuf, 4);

		int len = -1;
		std::istringstream iss(lenBuf);
		iss >> std::hex >> len;
		if (len < 0)
		{
			delete pResp;
			return NULL;
		}

		std::unique_ptr<char[]> msg(new char[len]);
		Read(client, msg.get(), len);

		pResp->message = msg.get();
	}

	return pResp;
}

bool AdbHelper::Read(SocketClient* client, char* data, int length)
{
	return Read(client, data, length, DdmPreferences::GetTimeOut());
}

bool AdbHelper::Read(SocketClient* client, char* data, int length, int timeout)
{
	int readCount = 0;
	int numWaits = 0;
	if (length <= 0)
	{
		return false;
	}

	while (readCount < length)
	{
		int count;

		count = client->Read(data, length);
		readCount += count;
		if (count < 0)
		{
			// error
			return false;
		}
		else if (count == 0)
		{
			if (timeout != 0 && numWaits * WAIT_TIME > timeout)
			{
				return false;
			}
			// non-blocking spin
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
			numWaits++;
		}
		else
		{
			numWaits = 0;
		}
	}
	return true;
}

bool AdbHelper::Write(SocketClient* client, const char* data, int length)
{
	return Write(client, data, length, DdmPreferences::GetTimeOut());
}

bool AdbHelper::Write(SocketClient* client, const char* data, int length, int timeout)
{
	int readCount = 0;
	int numWaits = 0;
	if (length <= 0)
	{
		length = (int) strlen(data);
	}

	while (readCount < length)
	{
		int count;

		count = client->Write(data);
		readCount += count;
		if (count < 0)
		{
			// error
			return false;
		}
		else if (count == 0)
		{
			if (timeout != 0 && numWaits * WAIT_TIME > timeout)
			{
				return false;
			}
			// non-blocking spin
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
			numWaits++;
		}
		else
		{
			numWaits = 0;
		}
	}
	return true;
}

bool AdbHelper::IsOkay(char* reply)
{
	return reply[0] == 'O' && reply[1] == 'K'
		&& reply[2] == 'A' && reply[3] == 'Y';
}
