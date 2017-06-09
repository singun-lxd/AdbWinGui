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
#include "../System/SocketClient.h"
#include "../System/StreamReader.h"
#include "IDevice.h"

#define ADB_SERVICE_COUT  2

class AdbHelper
{
private:
	AdbHelper();

public:
	struct AdbResponse
	{
		bool okay; // first 4 bytes in response were "OKAY"?
		std::string message; // diagnostic string if #okay is false
	};

	static const char* const s_arrAdbService[ADB_SERVICE_COUT];
	enum AdbService
	{
		SHELL,
		EXEC
	};
public:
	static int GetLastError();
	static int ReleaseSocket();
	static const char* FormAdbRequest(const char* req);	// need delete return string
	static AdbResponse* ReadAdbResponse(SocketClient* client, bool readDiagString);	// need delete return object
	static int ExecuteRemoteCommand(const SocketAddress& adbSockAddr,
		const TString command, IDevice* device, IShellOutputReceiver* rcvr, long maxTimeToOutputResponse);
	static int ExecuteRemoteCommand(const SocketAddress& adbSockAddr, AdbService adbService,
		const TString command, IDevice* device, IShellOutputReceiver* rcvr, long maxTimeToOutputResponse,
		CharStreamReader* reader);
	static bool Read(SocketClient* client, char* data, int length);
	static bool Read(SocketClient* client, char* data, int length, int timeout);
	static bool Write(SocketClient* client, const char* data, int length = -1);
	static bool Write(SocketClient* client, const char* data, int length, int timeout);
	static bool IsOkay(char* reply);
	static bool SetDevice(SocketClient* client, const IDevice* device);
};