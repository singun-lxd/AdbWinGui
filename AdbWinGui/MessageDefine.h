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

#define MSG_RIBBON_DEVICE_LIST		WM_USER + 0x100
#define MSG_RIBBON_INSTALL_APK		WM_USER + 0x101

#define MSG_MAIN_ADB_PATH			WM_USER + 0x200
#define MSG_MAIN_NOTIFY_EXIT			WM_USER + 0x201
#define MSG_MAIN_PREPARE_ADB			WM_USER + 0x202
#define MSG_MAIN_ADB_FINISH			WM_USER + 0x203
#define MSG_MAIN_INSTALL_APK			WM_USER + 0x204

#define MSG_INSTALL_APK_REQUEST		WM_USER + 0x300
#define MSG_INSTALL_APK_COPYED		WM_USER + 0x301
#define MSG_INSTALL_APK_FIISH		WM_USER + 0x302
#define MSG_INSTALL_STEP_PUSH		WM_USER + 0x303
#define MSG_INSTALL_STEP_PROGRESS	WM_USER + 0x304
#define MSG_INSTALL_STEP_INSTALL		WM_USER + 0x305

#define MSG_SETTING_SELECT_ADB		WM_USER + 0x400