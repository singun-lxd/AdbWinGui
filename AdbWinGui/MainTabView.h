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

// MainTabView.h: interface for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include "resource.h"
#include "MessageDefine.h"
#include "DdmLibWrapper.h"
#include "AdbPreparingDlg.h"

class MainTabView : public CTabViewImpl<MainTabView>, public DdmLibWrapper::DdmCallback
{
protected:
	enum
	{
		TAB_MAIN = 1,
		TAB_SETTING = 2,
	};
	CSimpleArray<CWindow*> m_arrWnd;
	AdbPreparingDlg m_dlgPreparing;
	DdmLibWrapper m_ddmLibWrapper;

public:
	DECLARE_WND_SUPERCLASS(NULL, CTabViewImpl<MainTabView>::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP_EX(MainTabView)
		CHAIN_MSG_MAP(CTabViewImpl<MainTabView>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(MSG_MAIN_NOTIFY_EXIT, OnNotifyExit)
		MESSAGE_HANDLER(MSG_MAIN_ADB_PATH, OnAdbPath)
		MESSAGE_HANDLER(MSG_MAIN_PREPARE_ADB, OnPrepareAdb)
		MESSAGE_HANDLER(MSG_MAIN_ADB_FINISH, OnAdbPrepareFinish)
	ALT_MSG_MAP(1)   // tab control
	END_MSG_MAP()

public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAdbPath(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPrepareAdb(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAdbPrepareFinish(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	virtual void InitFinish() override;
	virtual void OnDeviceUpdated(const CSimpleArray<Device>& arrDevice) override;

public:
	bool CreateTabControl();
	void UpdateLayout();
	void InitTabs();
	void DestroyTabs();

protected:
	void AddMainTab();
	void AddSettingTab();

private:
	void HandleAdbPathError(HWND hWndSetting);
	void HandleAdbPathSuccess(LPCTSTR lpszAdbPath);
};
