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

// MainTab.h : interface of the MainTab class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>
#include <atlframe.h>
#include "resource.h"

class MainTab : public CDialogImpl<MainTab>, public CDialogResize<MainTab>
{
public:
	enum {
		IDD = IDD_MAIN_TAB
	};

public:
	BEGIN_MSG_MAP(MainTab)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialogResize<MainTab>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(MainTab)
		DLGRESIZE_CONTROL(IDC_STATIC_PATH, DLSZ_CENTER_X | DLSZ_CENTER_Y)
	END_DLGRESIZE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void PrepareAdb();
};
