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

#include "SyncService.h"

class NotifySyncProgressMonitor : public SyncService::ISyncProgressMonitor
{
private:
	IDevice::ISyncNotify* m_pNotify;
	int m_nTotalWork;
	long m_lPushed;
public:
	NotifySyncProgressMonitor(IDevice::ISyncNotify* pNotify)
	{
		m_nTotalWork = 0;
		m_lPushed = 0;
		m_pNotify = pNotify;
	}

	void Advance(int work) override
	{
		m_lPushed += work;
		int nProgress = m_lPushed * 100 / m_nTotalWork;
		m_pNotify->OnProgress(nProgress);
	}

	bool IsCanceled() override
	{
		return m_pNotify->IsCancelled();
	}

	void Start(int totalWork) override
	{
		m_nTotalWork = totalWork;
	}

	void StartSubTask(const TString name) override
	{
	}

	void Stop() override
	{
	}
};