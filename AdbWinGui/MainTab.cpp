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
#include "Config/ConfigManager.h"
#include "Utils/ShellHelper.h"
#include "InstallNotifyDlg.h"

MainTab::MainTab() : m_ddmLibWrapper(DdmLibWrapper::GetInstance())
{
}

MainTab::~MainTab()
{
	if (m_taskInstall.valid())
	{
		m_taskInstall.get();
	}
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
		InstallNotify emNotify = ConfigManager::GetInstance().GetInstallNotifyConfig();
		switch (emNotify)
		{
		case em_InstallDefault:
			OnDefaultInstallDialog(szFilePathName);
			break;
		case em_InstallDirect:
			OnInstallApkDirect(szFilePathName);
			break;
		case em_InstallWithCopy:
			OnCopyAndInstallApk(szFilePathName);
			break;
		case em_InstallNone:
			break;
		}
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

void MainTab::OnDefaultInstallDialog(LPCTSTR lpszApkPath)
{
	BOOL bChecked = FALSE;
	InstallNotifyDlg dlg(lpszApkPath);
	int nClick = dlg.DoModal(m_hWnd, &bChecked);
	switch (nClick)
	{
	case InstallNotifyDlg::em_Button_Install_Direct:
		OnInstallApkDirect(lpszApkPath);
		break;
	case InstallNotifyDlg::em_Button_Install_With_Copy:
		OnCopyAndInstallApk(lpszApkPath);
		break;
	case InstallNotifyDlg::em_Button_Install_Cancel:
		break;
	}
}

void MainTab::OnInstallApkDirect(LPCTSTR lpszApkPath)
{
	IDevice* pDevice = m_ddmLibWrapper.GetSelectedDevice();
	if (pDevice != NULL)
	{
		// create a thread to run install task
		std::packaged_task<int()> ptInstall([&pDevice, &lpszApkPath]
		{
			return pDevice->InstallPackage(lpszApkPath, true);
		});
		m_taskInstall = ptInstall.get_future();
		std::thread(std::move(ptInstall)).detach();
	}
	else
	{

	}
}

void MainTab::OnCopyAndInstallApk(LPCTSTR lpszApkPath)
{
	LPCTSTR lpszFileName = ::PathFindFileName(lpszApkPath);
	CString strApkDir = ConfigManager::GetInstance().GetApkDir();
	LPTSTR szPath = strApkDir.GetBuffer(MAX_PATH);
	::PathAppend(szPath, lpszFileName);
	strApkDir.ReleaseBuffer();
	INT nRet = ShellHelper::CopyFile(lpszApkPath, strApkDir, m_hWnd);
	if (nRet == ERROR_SUCCESS)
	{
		OnInstallApkDirect(strApkDir);
	}
	else
	{

	}
}
