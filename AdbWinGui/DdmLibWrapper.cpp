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
#include "DdmLibWrapper.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/DDMLib.lib")
#else
#pragma comment(lib, "../Release/DDMLib.lib")
#endif

DdmLibWrapper::DdmLibWrapper()
{
	m_hModule = NULL;
	m_pAdbInstance = NULL;
	m_pCallback = NULL;
}

DdmLibWrapper::~DdmLibWrapper()
{
	m_pCallback = NULL;
	Release();

	if (m_hModule != NULL)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

void DdmLibWrapper::Init(const TString szLocation, BOOL bClienntSupport)
{
	if (m_pAdbInstance != NULL)
	{
		return;
	}
	std::packaged_task<BOOL(const TString, BOOL)> ptInit(std::bind(&DdmLibWrapper::InitAdb, this,
		std::placeholders::_1, std::placeholders::_2));
	m_taskInit = ptInit.get_future();
	std::thread(std::move(ptInit), szLocation, bClienntSupport).detach();
}

void DdmLibWrapper::Release()
{
	m_taskInit.get();
	if (m_pAdbInstance != NULL)
	{
		m_pAdbInstance->Release();
		m_pAdbInstance = NULL;
	}
}

BOOL DdmLibWrapper::IsInit()
{
	return m_pAdbInstance != NULL;
}

void DdmLibWrapper::SetDdmCallback(DdmCallback* pCallback)
{
	m_pCallback = pCallback;
}

void DdmLibWrapper::DeviceConnected(const IDevice* device)
{
	m_arrDevice.Add(const_cast<IDevice*>(device));

	NotifyDeviceChange();
}

void DdmLibWrapper::DeviceDisconnected(const IDevice* device)
{
	m_arrDevice.Remove(const_cast<IDevice*>(device));

	NotifyDeviceChange();
}

void DdmLibWrapper::DeviceChanged(const IDevice* device, int changeMask)
{
	int nIndex = m_arrDevice.Find(const_cast<IDevice*>(device));
	if (nIndex >= 0)
	{
		m_arrDevice[nIndex] = const_cast<IDevice*>(device);
	}

	NotifyDeviceChange();
}

BOOL DdmLibWrapper::InitAdb(const TString szLocation, BOOL bClientSupport)
{
	TCHAR szFileName[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szFileName, MAX_PATH);
	::PathRemoveFileSpec(szFileName);
	::PathAppend(szFileName, _T("DDMLib.dll"));
	m_hModule = ::LoadLibrary(szFileName);

	if (m_hModule != NULL)
	{
		m_pAdbInstance = ::GetDDMLibEntry(szLocation, bClientSupport ? true : false);
		m_pAdbInstance->SetDeviceChangeListener(this);

		if (m_pCallback != NULL)
		{
			m_pCallback->InitFinish();
		}
	}
	return FALSE;
}

void DdmLibWrapper::NotifyDeviceChange()
{
	if (m_pCallback != NULL)
	{
		m_pCallback->OnDeviceUpdated(m_arrDevice);
	}
}
