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
#include <atlcrack.h>
#include "resource.h"
#include "Config/ConfigManager.h"
#include "MessageDefine.h"
#include "CDialogColor.h"

class SettingTab : public CDialogImpl<SettingTab>,
					public CDialogResize<SettingTab>, public CDialogColor<SettingTab>
{
public:
	enum
	{
		IDD = IDD_SETTING_TAB
	};

	CButton m_btnRadioAuto;
	CButton m_btnRadioManual;
	CStatic m_stcAdbPath;
	CEdit m_edtAdbPath;

	CButton m_btnRadioCurrent;
	CButton m_btnRadioCustom;
	CStatic m_stcApkDir;
	CEdit m_edtApkDir;

public:
	BEGIN_MSG_MAP(SettingTab)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER_EX(MSG_SETTING_SELECT_ADB, OnShowSelectAdbDialog)
		COMMAND_RANGE_HANDLER_EX(IDC_RADIO_AUTO, IDC_RADIO_MANUAL, OnAdbRadioSelected)
		COMMAND_RANGE_HANDLER_EX(IDC_RADIO_CURRET, IDC_RADIO_CUSTOM, OnApkRadioSelected)
		CHAIN_MSG_MAP(CDialogResize<SettingTab>)
		CHAIN_MSG_MAP(CDialogColor<SettingTab>)
	END_MSG_MAP()
	
	BEGIN_DLGRESIZE_MAP(SettingTab)
		DLGRESIZE_CONTROL(IDC_FRAME_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FRAME_DIRECTORY, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_DIRECTORY, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_DIRECTORY, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnShowSelectAdbDialog(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnAdbRadioSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnApkRadioSelected(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
	void InitControls();
	void UpdateControlStatus();
	void CheckSettingValid();
	void SwitchToAutoMode(LPCTSTR lpszPath);
	void SwitchToManualMode(LPCTSTR lpszPath);
	void SwitchAdbRadioButton(BOOL bAuto);
	void AutoUpdateAdbPath(ConfigManager& cfgManager);
	BOOL ShowSelectAdbDialog(ConfigManager& cfgManager);
	void SwitchToCurrentMode(LPCTSTR lpszDir);
	void SwitchToCustomMode(LPCTSTR lpszDir);
	void SwitchApkRadioButton(BOOL bCurrent);
	void AutoUpdateApkDir(ConfigManager& cfgManager);
	BOOL ShowSelectApkDialog(ConfigManager& cfgManager);
};
