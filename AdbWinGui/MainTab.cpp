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
		}
	}
	else
	{
		MessageTaskDlg dlg(IDS_NOT_SUPPORTED_FILE, IDS_ONLY_APK_SUPPORTED, MB_ICONWARNING);
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
	InstallNotify emNotify = em_InstallDefault;
	BOOL bChecked = FALSE;
	InstallNotifyDlg dlg(lpszApkPath);
	int nClick = dlg.DoModal(m_hWnd, &bChecked);
	switch (nClick)
	{
	case InstallNotifyDlg::em_Button_Install_Direct:
		OnInstallApkDirect(lpszApkPath);
		emNotify = em_InstallDirect;
		break;
	case InstallNotifyDlg::em_Button_Install_With_Copy:
		OnCopyAndInstallApk(lpszApkPath);
		emNotify = em_InstallWithCopy;
		break;
	case InstallNotifyDlg::em_Button_Install_Cancel:
		break;
	}
	if (bChecked)
	{
		ConfigManager::GetInstance().SetInstallNotifyConfig(emNotify);
	}
}

void MainTab::OnInstallApkDirect(LPCTSTR lpszApkPath)
{
	TCHAR* szApkPath = new TCHAR[MAX_PATH];
	_tcsncpy_s(szApkPath, MAX_PATH, lpszApkPath, MAX_PATH);

	IDevice* pDevice = m_ddmLibWrapper.GetSelectedDevice();
	if (pDevice != NULL)
	{
		HWND hWnd = m_hWnd;
		// create a thread to run install task
		std::packaged_task<int(TCHAR*)> ptInstall([&pDevice, &hWnd](TCHAR* szApkPath)
		{
			int nRet = pDevice->InstallPackage(szApkPath, true);
			if (nRet == 0)
			{
			}
			// need to free string here
			delete[] szApkPath;
			return nRet;
		});
		m_taskInstall = ptInstall.get_future();
		std::thread(std::move(ptInstall), szApkPath).detach();
	}
	else
	{
		MessageTaskDlg dlg(IDS_NO_AVAILABLE_DEVICE, IDS_CONNECT_AND_RETRY, MB_ICONERROR);
		dlg.DoModal();
		delete[] szApkPath;
	}
}

void MainTab::OnCopyAndInstallApk(LPCTSTR lpszApkPath)
{
	TCHAR* szSrcPath = new TCHAR[MAX_PATH];
	_tcsncpy_s(szSrcPath, MAX_PATH, lpszApkPath, MAX_PATH);

	LPCTSTR lpszFileName = ::PathFindFileName(lpszApkPath);
	LPCTSTR lpszApkDir = ConfigManager::GetInstance().GetApkDir();
	TCHAR* szDescPath = new TCHAR[MAX_PATH];
	_tcsncpy_s(szDescPath, MAX_PATH, lpszApkDir, MAX_PATH);
	::PathAppend(szDescPath, lpszFileName);

	if (::PathFileExists(szDescPath))
	{
		ShowCopyFailDialog(ERROR_FILE_EXISTS);
		delete[] szSrcPath;
		delete[] szDescPath;
	}
	else
	{
		HWND hWnd = m_hWnd;
		// create a thread to run copy task
		std::packaged_task<BOOL(TCHAR*, TCHAR*)> ptCopy([&hWnd](TCHAR* lpszSrc, TCHAR* lpszDesc)
		{
			BOOL bRet = ::CopyFile(lpszSrc, lpszDesc, FALSE);
			if (bRet)
			{

			}
			// need to free string here
			delete[] lpszSrc;
			delete[] lpszDesc;
			return bRet;
		});
		m_taskInstall = ptCopy.get_future();
		std::thread(std::move(ptCopy), szSrcPath, szDescPath).detach();
	}
}

void MainTab::ShowCopyFailDialog(DWORD dwErrCode)
{
	LPCTSTR lpszErrMsg = ShellHelper::GetErrorMessage(dwErrCode);
	MessageTaskDlg dlg(IDS_FILE_COPY_FAILED, lpszErrMsg, MB_ICONERROR);
	dlg.DoModal();
}
