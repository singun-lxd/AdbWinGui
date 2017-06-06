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

#define MSG_MAIN_ADB_PATH			WM_USER + 100
#define MSG_MAIN_NOTIFY_EXIT			WM_USER + 101
#define MSG_MAIN_PREPARE_ADB			WM_USER + 102
#define MSG_MAIN_ADB_FINISH			WM_USER + 103

#define MSG_SETTING_SELECT_ADB		WM_USER + 200

#define MSG_RIBBON_DEVICE_LIST		WM_USER + 300
#define PARAM_DEVICE_CONNECT			0x01
#define PARAM_DEVICE_DISCONNECT		0x02
#define PARAM_DEVICE_CHANGE			0x03