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
#include "IShellEnabledDevice.h"

#define STATE_COUT  5

#define CHANGE_STATE			0x0001
#define CHANGE_CLIENT_LIST	0x0002;
#define CHANGE_BUILD_INFO	0x0004;

interface IDevice : public IShellEnabledDevice
{
public:
	static const TString const s_arrDevState[STATE_COUT];
	enum DeviceState
	{
		UNKNOWN = -1,
		BOOTLOADER = 0,
		OFFLINE,
		ONLINE,
		RECOVERY,
		UNAUTHORIZED,
	};

public:
	static DeviceState GetState(const TString state);

public:
	virtual const TString GetSerialNumber() const = 0;
	virtual DeviceState GetState() const = 0;
	virtual bool IsOnline() const = 0;
	virtual bool IsEmulator() const = 0;
	virtual bool IsOffline() const = 0;
	virtual bool IsBootLoader() const = 0;
};