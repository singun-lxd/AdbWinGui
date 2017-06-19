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
#include <atlcrack.h>
#include "resource.h"
#include "DdmLibWrapper.h"
#include "MessageDefine.h"
#include "CDialogColor.h"
#include "CDialogToolTip.h"
#include "../DDMLib/DDMLib/IDevice.h"

class MainTab : public CDialogImpl<MainTab>, public CDialogResize<MainTab>,
				public CDialogColor<MainTab>, public CDialogToolTip<MainTab>,
				public IDevice::IInstallNotify
{
public:
	enum
	{
		IDD = IDD_MAIN_TAB,
	};

	DdmLibWrapper& m_ddmLibWrapper;
	std::future<BOOL> m_taskInstall;
	std::future<BOOL> m_taskCopy;
	CStatic m_stcNoticeApk;
	CButton m_btnInstallApk;
	CStatic m_stcFilter;
	CEdit m_ediFilter;
	CButton m_btnRefresh;
	CProgressBarCtrl m_pgbInstall;
	CButton m_chkReistall;
	CStatic m_stcListInstall;
	CListViewCtrl m_lvApkDir;
	CImageList m_ilApkIcon;

	CBitmapHandle m_bmpApkIco;
	CBitmapHandle m_bmpRefresh;
	CSimpleArray<CString> m_arrApkPath;

	BOOL m_bIsCancelled;
	CString m_strErrMsg;

public:
	BEGIN_MSG_MAP(MainTab)
		CHAIN_MSG_MAP(CDialogToolTip<MainTab>)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DROPFILES(OnDropFiles)
		COMMAND_HANDLER_EX(IDC_EDIT_APK_FILTER, EN_CHANGE, OnEditFilterChange)
		COMMAND_ID_HANDLER_EX(IDC_BUTTON_REFRESH, OnBtnRefreshClick)
		COMMAND_ID_HANDLER_EX(IDC_BUTTON_INSTALL, OnBtnStopInstallClick)
		NOTIFY_HANDLER_EX(IDC_LIST_APK, LVN_KEYDOWN, OnListKeyDown)
		NOTIFY_HANDLER_EX(IDC_LIST_APK, NM_DBLCLK, OnListDblClick)
		MESSAGE_HANDLER_EX(MSG_INSTALL_APK, OnApkInstalled)
		MESSAGE_HANDLER_EX(MSG_INSTALL_COPY_APK, OnApkCopied)
		MESSAGE_HANDLER_EX(MSG_INSTALL_STEP_PUSH, OnInstallPush)
		MESSAGE_HANDLER_EX(MSG_INSTALL_STEP_PROGRESS, OnInstallProgress)
		MESSAGE_HANDLER_EX(MSG_INSTALL_STEP_INSTALL, OnInstallRun)
		CHAIN_MSG_MAP(CDialogResize<MainTab>)
		CHAIN_MSG_MAP(CDialogColor<MainTab>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(MainTab)
		DLGRESIZE_CONTROL(IDC_FRAME_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_LIST_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_PROGRESS_INSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_APK_FILTER, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_REFRESH, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_FILTER, DLSZ_MOVE_X)		
		DLGRESIZE_CONTROL(IDC_BUTTON_INSTALL, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_CHECK_REINSTALL, DLSZ_MOVE_X)		
	END_DLGRESIZE_MAP()

public:
	MainTab();
	~MainTab();
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnDropFiles(HDROP hDropInfo);
	void OnEditFilterChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnBtnRefreshClick(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnBtnStopInstallClick(UINT uNotifyCode, int nID, CWindow wndCtl);
	LRESULT OnListKeyDown(LPNMHDR pnmh);
	LRESULT OnListDblClick(LPNMHDR pnmh);
	LRESULT OnApkInstalled(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnApkCopied(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnInstallPush(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnInstallProgress(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnInstallRun(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// IDevice::IInstallNotify
	virtual void OnPush() override;
	virtual void OnProgress(int progress) override;
	virtual void OnInstall() override;
	virtual void OnRemove() override;
	virtual void OnErrorMessage(const TString errMsg);
	virtual bool IsCancelled() override;

private:
	void PrepareAdb();
	void InitControls();
	void OnDefaultInstallDialog(LPCTSTR lpszApkPath);
	void OnInstallApkDirect(LPCTSTR lpszApkPath);
	void OnCopyAndInstallApk(LPCTSTR lpszApkPath);
	void ShowFileOperationFailDialog(int nId, DWORD dwErrCode);
	void inline ShowCopyFailDialog(DWORD dwErrCode);
	void inline ShowDeleteFailDialog(DWORD dwErrCode);
	BOOL CheckAndShowReplaceDialog(LPCTSTR lpszFromPath, LPCTSTR lpszToPath);
	void SwitchToCopyingMode();
	void SwitchToInstallingMode();
	void SwitchToIdleMode();
	BOOL RefreshApkDirectory();
	BOOL RefreshApkDirectoryWithArray(CSimpleArray<CString>& arrApkPath);
	BOOL GetListItemApkPath(int nIndex, CString& strPath);
};
