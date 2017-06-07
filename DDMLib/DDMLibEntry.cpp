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

#include "stdafx.h"
#include "DDMLibEntry.h"

DDMLibEntry::DDMLibEntry(LPCTSTR lpszAdbPath, BOOL bClientSupport)
{
	AndroidDebugBridge::Init(bClientSupport);
	m_pBridge = &AndroidDebugBridge::CreateBridge(lpszAdbPath, true);
}

DDMLibEntry::~DDMLibEntry()
{
	AndroidDebugBridge::Terminate();
	m_pBridge = NULL;
}

void DDMLibEntry::SetDeviceChangeListener(AndroidDebugBridge::IDeviceChangeListener* pListener)
{
	if (m_pBridge != NULL)
	{
		m_pBridge->AddDeviceChangeListener(pListener);
	}
}

void DDMLibEntry::Release()
{
	delete this;
}

