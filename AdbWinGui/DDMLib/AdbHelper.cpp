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

#include <thread>
#include "AdbHelper.h"
#include "DdmPreferences.h"

#define WAIT_TIME		5 // spin-wait sleep, in ms

const byte* AdbHelper::FormAdbRequest(const char* req)
{
	std::stringstream ss;
	ss.fill(_T('0'));
	ss.width(4);
	ss << std::hex << strlen(req);
	ss << req;

	std::string& resultStr = ss.str();
	size_t length = resultStr.length();
	byte* result = new byte[length +1];
	memcpy(result, resultStr.c_str(), length * sizeof(char));
	result[length] = '\0';

	return result;
}

bool AdbHelper::Write(SocketClient* client, const byte* data)
{
	return Write(client, data, -1, DdmPreferences::GetTimeOut());
}

bool AdbHelper::Write(SocketClient* client, const byte* data, int length, int timeout)
{
	int readCount = 0;
	int numWaits = 0;
	if (length == -1)
	{
		length = strlen(reinterpret_cast<const char*>(data));
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
