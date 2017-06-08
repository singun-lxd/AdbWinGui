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
#include "..\System\SocketAddress.h"
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

public:
	SyncService(const SocketAddress& address, Device* device);

	bool OpenSync();
	void Close();

	static ISyncProgressMonitor* GetNullProgressMonitor();

	void PushFile(const TString local, const TString remote, ISyncProgressMonitor* monitor);
};