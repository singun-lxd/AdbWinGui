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
#include "MessageTaskDlg.h"

MainTab::MainTab() : m_ddmLibWrapper(DdmLibWrapper::GetInstance())
{
}

BOOL MainTab::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT MainTab::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DlgResize_Init(false, false, WS_CHILD);
	PrepareAdb();
	InitControls();
	return 0;
}

LRESULT MainTab::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDropInfo = (HDROP)wParam;
	TCHAR szFilePathName[MAX_PATH] = { 0 };
	::DragQueryFile(hDropInfo, 0, szFilePathName, MAX_PATH);
	::DragFinish(hDropInfo);

	LPCTSTR lpszExt = ::PathFindExtension(szFilePathName);
	if (_tcsicmp(_T(".APK"), lpszExt) == 0)
	{
	}
	else
	{
		MessageTaskDlg dlg(IDS_NOT_SUPPORTED_FILE, MB_ICONWARNING);
		dlg.DoModal();
	}

	return 0;
}

void MainTab::PrepareAdb()
{
	GetParent().PostMessage(MSG_MAIN_PREPARE_ADB);
}

void MainTab::InitControls()
{
	m_stcNoticeApk.Attach(GetDlgItem(IDC_STATIC_APK));
	m_btnInstallApk.Attach(GetDlgItem(IDC_BUTTON_INSTALL));
	m_pgbInstall.Attach(GetDlgItem(IDC_PROGRESS_INSTALL));
}
