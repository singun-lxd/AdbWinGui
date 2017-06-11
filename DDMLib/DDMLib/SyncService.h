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
#include "../System/SocketAddress.h"
#include "../System/File.h"
#include "Device.h"

// define class
class Device;

class SyncService
{
public:
	interface ISyncProgressMonitor
	{
		virtual void Start(int totalWork) = 0;
		virtual void Stop() = 0;
		virtual bool IsCanceled() = 0;
		virtual void StartSubTask(const TString name) = 0;
		virtual void Advance(int work) = 0;
	};

	struct FileStat
	{
	private:
		const int m_nMode;
		const int m_nSize;
		const time_t m_tLastModified;

	public:
		FileStat(int mode, int size, int lastModifiedSecs);
		int GetMode() const;
		int GetSize() const;
		time_t GetLastModified() const;
	};

private:
	class NullSyncProgressMonitor : public ISyncProgressMonitor
	{
		void Advance(int work) override {}
		bool IsCanceled() override { return false; }
		void Start(int totalWork) override {}
		void StartSubTask(const TString name) override {}
		void Stop() override {}
	};

private:
	static NullSyncProgressMonitor* const s_pNullSyncProgressMonitor;

	SocketAddress m_socketAddress;
	Device* m_pDevice;
	SocketClient* m_pClient;
	char* m_pBuffer;

public:
	SyncService(const SocketAddress& address, Device* device);
	~SyncService();

	bool OpenSync();
	void Close();

	static ISyncProgressMonitor* GetNullProgressMonitor();

	bool PushFile(const TString local, const TString remote, ISyncProgressMonitor* monitor);
	bool PullFile(const TString remote, const TString local, ISyncProgressMonitor* monitor);
	bool StatFile(const TString path, FileStat** fileStat);

private:
	bool DoPushFile(const File& file, const TString remotePath, ISyncProgressMonitor* monitor);
	bool DoPullFile(const TString remotePath, const TString localPath, ISyncProgressMonitor* monitor);
	static char* CreateReq(const char* command, int value, int& len);
	static char* CreateFileReq(const char* command, const TString path, int& len);
	static char* CreateSendFileReq(const char* command, const TString path, int mode, int& len);
	static bool CheckResult(char* result, char* code);
	char* GetBuffer();
};