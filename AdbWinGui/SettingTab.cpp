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

// SettingTab.cpp : implement of the SettingTab class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SettingTab.h"
#include "MessageDefine.h"
#include "MessageTaskDlg.h"
#include "DDMLib/AndroidDebugBridge.h"

BOOL SettingTab::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

LRESULT SettingTab::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DlgResize_Init(false, false, WS_CHILD);
	InitControls();
	UpdateControlStatus();
	CheckSettingValid();

	return FALSE;
}

LRESULT SettingTab::OnShowSelectAdbDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	BOOL bRet = ShowSelectAdbDialog(cfgManager);
	if (bRet == TRUE)
	{
		SwitchRadioButton(FALSE);
	}
	else
	{
		GetParent().PostMessage(MSG_MAIN_NOTIFY_EXIT);
	}

	return 0;
}

LRESULT SettingTab::OnRadioSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	switch (wID)
	{
	case IDC_RADIO_AUTO:
		AutoUpdateAdbPath(cfgManager);
		break;
	case IDC_RADIO_MANUAL:
		ShowSelectAdbDialog(cfgManager);
		break;
	}
	return 0;
}

// LRESULT SettingTab::OnPathClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
// {
// 	ConfigManager& cfgManager = ConfigManager::GetInstance();
// 	ShowSelectAdbDialog(cfgManager);
// 	return 0;
// }

void SettingTab::InitControls()
{
	m_btnRadioAuto.Attach(GetDlgItem(IDC_RADIO_AUTO));
	m_btnRadioManual.Attach(GetDlgItem(IDC_RADIO_MANUAL));
	m_stcAdbPath.Attach(GetDlgItem(IDC_STATIC_PATH));
	m_edtAdbPath.Attach(GetDlgItem(IDC_EDIT_PATH));
}

void SettingTab::UpdateControlStatus()
{
	// read setting file and update UI
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	const CString& strAdbPath = cfgManager.GetAdbPath();

	if (cfgManager.GetAdbPathMode() == AdbPathConfig::em_PathModeAuto)
	{
		m_btnRadioAuto.SetCheck(TRUE);
		SwitchToAutoMode(strAdbPath);
	}
	else
	{
		m_btnRadioManual.SetCheck(TRUE);
		SwitchToManualMode(strAdbPath);
	}
}

void SettingTab::CheckSettingValid()
{
	const CString& strAdbPath = ConfigManager::GetInstance().GetAdbPath();
	//AndroidDebugBridge::Init(false); 
	//AndroidDebugBridge::CreateBridge(strAdbPath);
	//AndroidDebugBridge::Terminate();
	if (strAdbPath.IsEmpty())
	{
		CWindow hWnd = GetParent();
		hWnd.PostMessage(MSG_MAIN_ADB_ERROR, 0, (LPARAM)m_hWnd);
	}
}

void SettingTab::SwitchToAutoMode(LPCTSTR lpszePath)
{
	m_stcAdbPath.SetWindowText(lpszePath);
	m_stcAdbPath.EnableWindow(TRUE);
	m_edtAdbPath.EnableWindow(FALSE);
	m_edtAdbPath.SetWindowText(_T(""));
}

void SettingTab::SwitchToManualMode(LPCTSTR lpszePath)
{
	m_stcAdbPath.SetWindowText(_T(""));
	m_stcAdbPath.EnableWindow(FALSE);
	m_edtAdbPath.EnableWindow(TRUE);
	m_edtAdbPath.SetWindowText(lpszePath);
}

void SettingTab::SwitchRadioButton(BOOL bAuto)
{
	CheckRadioButton(IDC_RADIO_AUTO, IDC_RADIO_MANUAL, bAuto ? IDC_RADIO_AUTO : IDC_RADIO_MANUAL);
	SetFocus();	// set focus on the tab to avoid infinite messages
}

void SettingTab::AutoUpdateAdbPath(ConfigManager& cfgManager)
{
	const CString& strPath = cfgManager.AutoAdbPath();
	if (!strPath.IsEmpty())
	{
		SwitchToAutoMode(strPath);
		cfgManager.SetAdbPath(_T(""));
	}
	else
	{
		SwitchRadioButton(FALSE);

		CString strMsg;
		strMsg.LoadString(IDS_ADB_PATH_ERROR);
		MessageTaskDlg msgDlg;
		msgDlg.DoModal(m_hWnd, strMsg, MB_ICONEXCLAMATION);
	}
}

BOOL SettingTab::ShowSelectAdbDialog(ConfigManager& cfgManager)
{
	CFileDialog fileDialog(TRUE, _T("exe"), _T("adb.exe"),
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("adb.exe\0adb.exe\0\0"),
		GetParent());
	if (fileDialog.DoModal() == IDOK)
	{
		LPCTSTR lpszPath = fileDialog.m_ofn.lpstrFile;
		SwitchToManualMode(lpszPath);
		cfgManager.SetAdbPath(lpszPath);
		return TRUE;
	}
	else
	{
		SwitchRadioButton(TRUE);
		return FALSE;
	}
}
