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

#include "SyncService.h"
#include "..\System\StreamReader.h"
#include "DdmPreferences.h"
#include "AdbHelper.h"

#define SYNC_DATA_MAX				64*1024
#define REMOTE_PATH_MAX_LENGTH	1024

SyncService::NullSyncProgressMonitor* const SyncService::s_pNullSyncProgressMonitor = new NullSyncProgressMonitor();

SyncService::SyncService(const SocketAddress& address, Device* device)
{

}

bool SyncService::OpenSync()
{
	return false;
}

void SyncService::Close()
{

}

SyncService::ISyncProgressMonitor* SyncService::GetNullProgressMonitor()
{
	return s_pNullSyncProgressMonitor;
}

bool SyncService::PushFile(const TString local, const TString remote, ISyncProgressMonitor* monitor)
{
	File file(local);
	if (!file.Exists())
	{
		return false;
	}

	if (file.IsDirectory())
	{
		return false;
	}

	monitor->Start(static_cast<int>(file.GetLength()));

	DoPushFile(file, remote, monitor);

	monitor->Stop();

	return true;
}

bool SyncService::DoPushFile(const File& file, const TString remotePath, ISyncProgressMonitor* monitor)
{
// 	char* msg = NULL;
// 
// 	const int timeOut = DdmPreferences::GetTimeOut();
// 
// 	int pathLen = _tcslen(remotePath);
// 	if (pathLen > REMOTE_PATH_MAX_LENGTH)
// 	{
// 		return false;
// 	}
// 
// 	FileReadWrite fRead = file.GetRead();
// 
// 	const int nBuffSize = 1024;
// 	// create the stream to read the file
// 	CharStreamReader fsr(fRead, nBuffSize);
// 
// 	// create the header for the action
// 	msg = CreateSendFileReq(ID_SEND, remotePathContent, 0644);
// 
// 	// and send it. We use a custom try/catch block to make the difference between
// 	// file and network IO exceptions.
// 	AdbHelper::Write(mChannel, msg, -1, timeOut);
// 
// 	System.arraycopy(ID_DATA, 0, getBuffer(), 0, ID_DATA.length);
// 
// 	// look while there is something to read
// 	while (true)
// 	{
// 		// check if we're canceled
// 		if (monitor->IsCanceled())
// 		{
// 			return false;
// 		}
// 
// 		// read up to SYNC_DATA_MAX
// 		int readCount = fsr.ReadData(GetBuffer(), 8, SYNC_DATA_MAX);
// 
// 		if (readCount == -1)
// 		{
// 			// we reached the end of the file
// 			break;
// 		}
// 
// 		// now send the data to the device
// 		// first write the amount read
// 		ArrayHelper.swap32bitsToArray(readCount, getBuffer(), 4);
// 
// 		// now write it
// 		AdbHelper::Write(mChannel, getBuffer(), readCount + 8, timeOut);
// 
// 		// and advance the monitor
// 		monitor->Advance(readCount);
// 	}
// 
// 	// close the local file
// 	fRead.Close();
// 	fRead.Delete();
// 
// 	// create the DONE message
// 	long time = file.GetLastModifiedTime() / 1000;
// 	msg = CreateReq(ID_DONE, (int)time);
// 
// 	// and send it.
// 	AdbHelper::Write(mChannel, msg, -1, timeOut);
// 
// 	// read the result, in a byte array containing 2 ints
// 	// (id, size)
// 	char result[8];
// 	AdbHelper::Read(mChannel, result, -1 /* full length */, timeOut);
// 
// 	if (!checkResult(result, ID_OKAY))
// 	{
// 		return false;
// 	}
 	return true;
}
