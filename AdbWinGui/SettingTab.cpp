// SettingTab.cpp : implement of the SettingTab class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atldlgs.h>
#include "SettingTab.h"

BOOL SettingTab::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

LRESULT SettingTab::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	InitControls();
	UpdateControlStatus();

	return FALSE;
}

LRESULT SettingTab::OnRadioSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ConfigManager& cfgManager = ConfigManager::GetInstance();
	switch (wID)
	{
	case IDC_RADIO_AUTO:
		SwitchToAutoMode(cfgManager.UpdateAdbPath());
		cfgManager.SetAdbPath(_T(""));
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

void SettingTab::ShowSelectAdbDialog(ConfigManager& cfgManager)
{
	CFileDialog fileDialog(TRUE, _T("exe"), _T("adb.exe"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("adb.exe\0adb.exe\0\0"),
		GetParent());
	if (fileDialog.DoModal() == IDOK)
	{
		LPCTSTR lpszPath = fileDialog.m_ofn.lpstrFile;
		SwitchToManualMode(lpszPath);
		cfgManager.SetAdbPath(lpszPath);
	}
	else
	{
		m_btnRadioAuto.SetCheck(TRUE);
		m_btnRadioManual.SetCheck(FALSE);
		SetFocus();	// set focus on the tab to avoid infinite messages
	}
}
