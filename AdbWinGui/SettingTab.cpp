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

#define ADB_FILE_NNAME		_T("adb.exe")

BOOL SettingTab::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

BOOL SettingTab::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	DlgResize_Init(false, false, WS_CHILD);
	InitControls();
	UpdateControlStatus();
	CheckSettingValid();

	return FALSE;
}

LRESULT SettingTab::OnShowSelectAdbDialog(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	BOOL bRet = ShowSelectAdbDialog(cfgManager);
	if (bRet == TRUE)
	{
		SwitchAdbRadioButton(FALSE);
	}
	else
	{
		GetParent().PostMessage(MSG_MAIN_NOTIFY_EXIT);
	}

	return 0;
}

void SettingTab::OnAdbRadioSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	switch (nID)
	{
	case IDC_RADIO_AUTO:
		AutoUpdateAdbPath(cfgManager);
		break;
	case IDC_RADIO_MANUAL:
		ShowSelectAdbDialog(cfgManager);
		break;
	}
}

void SettingTab::OnApkRadioSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	switch (nID)
	{
	case IDC_RADIO_CURRET:
		AutoUpdateApkDir(cfgManager);
		break;
	case IDC_RADIO_CUSTOM:
		ShowSelectApkDialog(cfgManager);
		break;
	}
}

void SettingTab::InitControls()
{
	m_btnRadioAuto.Attach(GetDlgItem(IDC_RADIO_AUTO));
	m_btnRadioManual.Attach(GetDlgItem(IDC_RADIO_MANUAL));
	m_stcAdbPath.Attach(GetDlgItem(IDC_STATIC_PATH));
	m_edtAdbPath.Attach(GetDlgItem(IDC_EDIT_PATH));

	m_btnRadioCurrent.Attach(GetDlgItem(IDC_RADIO_CURRET));
	m_btnRadioCustom.Attach(GetDlgItem(IDC_RADIO_CUSTOM));
	m_stcApkDir.Attach(GetDlgItem(IDC_STATIC_DIRECTORY));
	m_edtApkDir.Attach(GetDlgItem(IDC_EDIT_DIRECTORY));
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

	const CString& strApkDir = cfgManager.GetApkDir();
	if (cfgManager.GetApkDirMode() == ApkDirectoryConfig::em_DirModeCurrent)
	{
		m_btnRadioCurrent.SetCheck(TRUE);
		SwitchToCurrentMode(strApkDir);
	}
	else
	{
		m_btnRadioCustom.SetCheck(TRUE);
		SwitchToCustomMode(strApkDir);
	}
}

void SettingTab::CheckSettingValid()
{
	const CString& strAdbPath = ConfigManager::GetInstance().GetAdbPath();
	if (strAdbPath.IsEmpty())
	{
		CWindow hWnd = GetParent();
		hWnd.PostMessage(MSG_MAIN_ADB_PATH, FALSE, (LPARAM)m_hWnd);
	}
	else
	{
		CWindow hWnd = GetParent();
		hWnd.PostMessage(MSG_MAIN_ADB_PATH, TRUE, (LPARAM)(LPCTSTR)strAdbPath);
	}
}

void SettingTab::SwitchToAutoMode(LPCTSTR lpszPath)
{
	m_stcAdbPath.SetWindowText(lpszPath);
	m_stcAdbPath.EnableWindow(TRUE);
	m_edtAdbPath.EnableWindow(FALSE);
	m_edtAdbPath.SetWindowText(_T(""));
}

void SettingTab::SwitchToManualMode(LPCTSTR lpszPath)
{
	m_stcAdbPath.SetWindowText(_T(""));
	m_stcAdbPath.EnableWindow(FALSE);
	m_edtAdbPath.EnableWindow(TRUE);
	m_edtAdbPath.SetWindowText(lpszPath);
}

void SettingTab::SwitchAdbRadioButton(BOOL bAuto)
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
		SwitchAdbRadioButton(FALSE);

		MessageTaskDlg msgDlg(IDS_ADB_PATH_ERROR, _T(""), MB_ICONEXCLAMATION);
		msgDlg.DoModal();
	}
}

BOOL SettingTab::ShowSelectAdbDialog(ConfigManager& cfgManager)
{
	COMDLG_FILTERSPEC fileSpec[] =
	{
		{ ADB_FILE_NNAME, ADB_FILE_NNAME },
	};

	CShellFileOpenDialog fileDialog(ADB_FILE_NNAME, FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST, _T("exe"), fileSpec, _countof(fileSpec));
	CString strTitle;
	strTitle.LoadString(IDS_SELECT_ADB_PATH);
	fileDialog.m_spFileDlg->SetTitle(strTitle);
	if (fileDialog.DoModal() == IDOK)
	{
		CString strFilePath;
		fileDialog.GetFilePath(strFilePath);
		SwitchToManualMode(strFilePath);
		cfgManager.SetAdbPath(strFilePath);
		return TRUE;
	}
	else
	{
		SwitchAdbRadioButton(TRUE);
		return FALSE;
	}
}

void SettingTab::SwitchToCurrentMode(LPCTSTR lpszDir)
{
	m_stcApkDir.SetWindowText(lpszDir);
	m_stcApkDir.EnableWindow(TRUE);
	m_edtApkDir.EnableWindow(FALSE);
	m_edtApkDir.SetWindowText(_T(""));
}

void SettingTab::SwitchToCustomMode(LPCTSTR lpszDir)
{
	m_stcApkDir.SetWindowText(_T(""));
	m_stcApkDir.EnableWindow(FALSE);
	m_edtApkDir.EnableWindow(TRUE);
	m_edtApkDir.SetWindowText(lpszDir);
}

void SettingTab::SwitchApkRadioButton(BOOL bCurrent)
{
	CheckRadioButton(IDC_RADIO_CURRET, IDC_RADIO_CUSTOM, bCurrent ? IDC_RADIO_CURRET : IDC_RADIO_CUSTOM);
	SetFocus();	// set focus on the tab to avoid infinite messages
}

void SettingTab::AutoUpdateApkDir(ConfigManager& cfgManager)
{
	const CString& strPath = cfgManager.CurrentApkDir();
	if (!strPath.IsEmpty())
	{
		SwitchToCurrentMode(strPath);
		cfgManager.SetApkDir(_T(""));
	}
	else
	{
		SwitchApkRadioButton(FALSE);

		MessageTaskDlg msgDlg(IDS_ADB_PATH_ERROR, _T(""), MB_ICONEXCLAMATION);
		msgDlg.DoModal();
	}
}

BOOL SettingTab::ShowSelectApkDialog(ConfigManager& cfgManager)
{
	CShellFileOpenDialog fileDialog(NULL, FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);
	CString strTitle;
	strTitle.LoadString(IDS_SELECT_APK_DIRECTORY);
	fileDialog.m_spFileDlg->SetTitle(strTitle);
	if (fileDialog.DoModal() == IDOK)
	{
		CString strDirPath;
		fileDialog.GetFilePath(strDirPath);
		SwitchToCustomMode(strDirPath);
		cfgManager.SetApkDir(strDirPath);
		return TRUE;
	}
	else
	{
		SwitchApkRadioButton(TRUE);
		return FALSE;
	}
}
