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

#include "../DDMLib/DDMLib.h"

class DdmLibWrapper : public AndroidDebugBridge::IDeviceChangeListener
{
public:
	interface DdmCallback
	{
		virtual void OnDeviceUpdated(const CSimpleArray<IDevice*>& arrDevice) = 0;
		virtual void InitFinish() = 0;
	};

private:
	HMODULE m_hModule;
	IDDMLibEntry* m_pAdbInstance;
	std::future<BOOL> m_taskInit;
	DdmCallback* m_pCallback;
	CSimpleArray<IDevice*> m_arrDevice;
	int m_nSelectedIndex;

	static DdmLibWrapper s_libDdm;

private:
	DdmLibWrapper();
public:
	~DdmLibWrapper();

	static DdmLibWrapper& GetInstance();

	void Init(const TString szLocation, BOOL bClienntSupport = FALSE);
	void Release();
	BOOL IsInit();
	void SetDdmCallback(DdmCallback* pCallback);

	virtual void DeviceConnected(const IDevice* device) override;
	virtual void DeviceDisconnected(const IDevice* device) override;
	virtual void DeviceChanged(const IDevice* device, int changeMask) override;

	void SelectDevice(int nIndex);
	int GetSelectedDeviceIndex();
	IDevice* GetSelectedDevice();

private:
	BOOL InitAdb(const TString szLocation, BOOL bClienntSupport);
	void NotifyDeviceChange();
};