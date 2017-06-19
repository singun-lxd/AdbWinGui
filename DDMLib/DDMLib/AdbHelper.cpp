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
#include "Log.h"
#include "StringUtils.h"

#define DDMS			_T("ddms")
#define WAIT_TIME		5 // spin-wait sleep, in ms

const char* const AdbHelper::s_arrAdbService[ADB_SERVICE_COUT] =
{
	"shell",
	"exec"
};

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
	strcpy(result, resultStr.c_str());

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

		std::unique_ptr<char[]> msg(new char[len + 1]);
		Read(client, msg.get(), len);
		msg[len] = '\0';

		pResp->message = msg.get();
	}

	return pResp;
}

int AdbHelper::ExecuteRemoteCommand(const SocketAddress& adbSockAddr, const TString command,
	IDevice* device, IShellOutputReceiver* rcvr, long maxTimeToOutputResponse)
{
	return ExecuteRemoteCommand(adbSockAddr, AdbService::SHELL, command, device, rcvr, maxTimeToOutputResponse,
		NULL/* StreamReader */);
}

int AdbHelper::ExecuteRemoteCommand(const SocketAddress& adbSockAddr, AdbService adbService, const TString command,
	IDevice* device, IShellOutputReceiver* rcvr, long maxTimeToOutputResponse, CharStreamReader* reader)
{
	LogVEx(DDMS, _T("execute: running %s"), command);

	std::unique_ptr<SocketClient> adbClient(SocketClient::Open(adbSockAddr));
	if (!adbClient)
	{
		return -1;
	}
	adbClient->ConfigureBlocking(false);

	// if the device is not -1, then we first tell adb we're looking to
	// talk
	// to a specific device
	bool bRet = SetDevice(adbClient.get(), device);
	if (!bRet)
	{
		return -1;
	}

	const char* enumValue = s_arrAdbService[static_cast<int>(adbService)];
	const char* szCommand = NULL;
#ifdef _UNICODE
	std::string strCmd;
	ConvertUtils::WstringToString(command, strCmd);
	szCommand = strCmd.c_str();
#else
	szCommand = command;
#endif
	std::ostringstream oss;
	oss << enumValue << ":" << szCommand;
	std::string& resultStr = oss.str();
	std::unique_ptr<const char[]> request(FormAdbRequest(resultStr.c_str()));
	bRet = Write(adbClient.get(), request.get());
	if (!bRet)
	{
		return -1;
	}

	std::unique_ptr<AdbResponse> resp(ReadAdbResponse(adbClient.get(), false /* readDiagString */));
	if (!resp || !resp->okay)
	{
		LogEEx(DDMS, _T("ADB rejected shell command (%s): %s"), command, resp->message);
		return -1;
	}

	const int bufferLen = 16384;
	char data[bufferLen] = { 0 };

	// stream the input file if present.
	if (reader != NULL)
	{
		int read;
		while ((read = reader->ReadData(data, bufferLen)) != -1)
		{
			int written = 0;
			written += adbClient->Write(data, read);
			if (written != read)
			{
				LogEEx(DDMS, _T("ADB write inconsistency, wrote %d expected %d"), written, read);
				return -1;
			}
		}
	}

	ZeroMemory(data, sizeof(char) * bufferLen);
	long timeToResponseCount = 0;
	int err = 0;
	while (true)
	{
		int count;

		if (rcvr != NULL && rcvr->IsCancelled())
		{
			LogV(DDMS, _T("execute: cancelled"));
			break;
		}

		count = adbClient->Read(data, bufferLen);
		if (count < 0)
		{
			err = GetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS)
			{
				int wait = WAIT_TIME * 5;
				timeToResponseCount += wait;
				if (maxTimeToOutputResponse > 0 && timeToResponseCount > maxTimeToOutputResponse)
				{
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(wait));
			}
			else
			{
				break;
			}
		}
		else if (count == 0)
		{
			err = 0;
			// we're at the end, we flush the output
			rcvr->Flush();
			LogVEx(DDMS, _T("execute '%s' on '%s' : EOF hit. Read: %d"),
				command, device->GetSerialNumber(), count);
			break;
		}
		else
		{
			// reset timeout
			timeToResponseCount = 0;

			// send data to receiver if present
			if (rcvr != NULL)
			{
				if (count < bufferLen - 1)
				{
					data[count] = '\0';
				}
				rcvr->AddOutput(data, 0, count);
			}
		}
	}
	if (adbClient)
	{
		adbClient->Close();
	}
	LogV(DDMS, _T("execute: returning"));
	if (err != 0)
	{
		return -1;
	}
	return 0;
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
		if (count < 0)
		{
			int err = GetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS)
			{
				if (timeout != 0 && numWaits * WAIT_TIME > timeout)
				{
					LogD(DDMS, _T("read: timeout"));
					return false;
				}
				// non-blocking spin
				std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
				numWaits++;
			}
			else
			{
				// error
				LogDEx(DDMS, _T("read: channel error %d"), GetLastError());
				return false;
			}
		}
		else if (count == 0)
		{
			// normal finished
			break;
		}
		else
		{
			numWaits = 0;
		}
		readCount += count;
	}
	return true;
}

bool AdbHelper::Write(SocketClient* client, const char* data, int length)
{
	return Write(client, data, length, DdmPreferences::GetTimeOut());
}

bool AdbHelper::Write(SocketClient* client, const char* data, int length, int timeout)
{
	int writeCount = 0;
	int numWaits = 0;
	if (length <= 0)
	{
		length = (int) strlen(data);
	}

	while (writeCount < length)
	{
		int count;

		count = client->Write(data, length);
		if (count < 0)
		{
			int err = GetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS)
			{
				if (timeout != 0 && numWaits * WAIT_TIME > timeout)
				{
					LogD(DDMS, _T("write: timeout"));
					return false;
				}
				// non-blocking spin
				std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
				numWaits++;
			}
			else
			{
				// error
				LogDEx(DDMS, _T("write: channel error %d"), GetLastError());
				return false;
			}
		}
		else if (count == 0)
		{
			// normal finished
			break;
		}
		else
		{
			numWaits = 0;
		}
		writeCount += count;
	}
	return true;
}

bool AdbHelper::IsOkay(char* reply)
{
	return reply[0] == 'O' && reply[1] == 'K'
		&& reply[2] == 'A' && reply[3] == 'Y';
}

bool AdbHelper::SetDevice(SocketClient* client, const IDevice* device)
{
	// if the device is not -1, then we first tell adb we're looking to talk
	// to a specific device
	if (device != NULL)
	{
		const char* serialNumber;
#ifdef _UNICODE
		std::string strSn;
		ConvertUtils::WstringToString(device->GetSerialNumber(), strSn);
		serialNumber = strSn.c_str();
#else
		serialNumber = NULL;
#endif
		std::ostringstream oss;
		oss << "host:transport:" << serialNumber;
		std::unique_ptr<const char[]> device_query(FormAdbRequest(oss.str().c_str()));

		bool bRet = Write(client, device_query.get());
		if (bRet)
		{
			std::unique_ptr<AdbResponse> resp(ReadAdbResponse(client, false /* readDiagString */));
			if (!resp || !resp->okay)
			{
				// request was refused by adb!
				bRet = false;
			}
			return bRet;
		}
	}
	return false;
}