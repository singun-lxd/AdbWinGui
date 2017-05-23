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

// MainTab.cpp : implement of the MainTab class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTab.h"
#include "MessageDefine.h"

BOOL MainTab::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT MainTab::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DlgResize_Init(false, false, WS_CHILD);
	PrepareAdb();
	return 0;
}

void MainTab::PrepareAdb()
{
	//GetParent().PostMessage(MSG_MAIN_PREPARE_ADB);
}
