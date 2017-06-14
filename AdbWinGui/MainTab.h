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
#include "DdmLibWrapper.h"

class MainTab : public CDialogImpl<MainTab>, public CDialogResize<MainTab>
{
public:
	enum
	{
		IDD = IDD_MAIN_TAB
	};

	DdmLibWrapper& m_ddmLibWrapper;
	std::future<BOOL> m_taskInstall;
	CStatic m_stcNoticeApk;
	CButton m_btnInstallApk;
	CProgressBarCtrl m_pgbInstall;

public:
	BEGIN_MSG_MAP(MainTab)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
		CHAIN_MSG_MAP(CDialogResize<MainTab>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(MainTab)
		DLGRESIZE_CONTROL(IDC_FRAME_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_LIST_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_APK, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_PROGRESS_INSTALL, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_APK_FILTER, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_STATIC_FILTER, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_INSTALL, DLSZ_MOVE_X)
	END_DLGRESIZE_MAP()

public:
	MainTab();
	~MainTab();
	BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void PrepareAdb();
	void InitControls();
	void OnDefaultInstallDialog(LPCTSTR lpszApkPath);
	void OnInstallApkDirect(LPCTSTR lpszApkPath);
	void OnCopyAndInstallApk(LPCTSTR lpszApkPath);
};
