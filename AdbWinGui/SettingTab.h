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

// SettingTab.h : interface of the SettingTab class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>
#include <atlframe.h>
#include "resource.h"
#include "Config/ConfigManager.h"
#include "MessageDefine.h"

class SettingTab : public CDialogImpl<SettingTab>, public CDialogResize<SettingTab>
{
public:
	enum {
		IDD = IDD_SETTING_TAB
	};

	CButton m_btnRadioAuto;
	CButton m_btnRadioManual;
	CStatic m_stcAdbPath;
	CEdit m_edtAdbPath;

public:
	BEGIN_MSG_MAP(SettingTab)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(MSG_SETTING_SELECT_ADB, OnShowSelectAdbDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO_AUTO, IDC_RADIO_MANUAL, OnRadioSelected)
		//COMMAND_ID_HANDLER(IDC_EDIT_PATH, OnPathClick)
		CHAIN_MSG_MAP(CDialogResize<SettingTab>)
	END_MSG_MAP()
	
	BEGIN_DLGRESIZE_MAP(MainTab)
		DLGRESIZE_CONTROL(IDC_FRAME_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_PATH, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowSelectAdbDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRadioSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnPathClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	void InitControls();
	void UpdateControlStatus();
	void CheckSettingValid();
	void SwitchToAutoMode(LPCTSTR lpszePath);
	void SwitchToManualMode(LPCTSTR lpszePath);
	void SwitchRadioButton(BOOL bAuto);
	void AutoUpdateAdbPath(ConfigManager& cfgManager);
	BOOL ShowSelectAdbDialog(ConfigManager& cfgManager);
};
