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

// MainTabView.cpp: implement for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTabView.h"
#include "MainTab.h"
#include "SettingTab.h"
#include "AdbPathErrorDlg.h"

#define	 MIN_SHOW_TIME		500
#define	 MIN_DIALOG_TIME	500
#define	 INIT_TIMER_ID		0x20
#define	 WAIT_TIMER_ID		0x40

BOOL MainTabView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

MainTabView::MainTabView() : m_ddmLibWrapper(DdmLibWrapper::GetInstance())
{
	m_dwDlgStartTime = 0;
	m_bPrepareFinish = FALSE;
}

int MainTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

void MainTabView::OnDestroy()
{
	BOOL bHandled = false;
	CTabViewImpl<MainTabView>::OnDestroy(WM_DESTROY, 0, 0, bHandled);
	if (bHandled)
	{
		SetMsgHandled(TRUE);
	}
	DestroyTabs();
}

void MainTabView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case INIT_TIMER_ID:
		KillTimer(INIT_TIMER_ID);
		if (!m_bPrepareFinish)
		{
			m_dwDlgStartTime = ::GetTickCount();
			int nRet = m_dlgPreparing.DoModal();
			if (nRet == IDCANCEL)
			{
				// todo dialog canceled
			}
			else
			{
				// todo dialog finished
			}
		}
		break;
	case WAIT_TIMER_ID:
		KillTimer(WAIT_TIMER_ID);
		if (m_dlgPreparing.IsShowing())
		{
			m_dlgPreparing.EndDialog(IDOK);
		}
		break;
	}
}

LRESULT MainTabView::OnNotifyExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	GetParent().PostMessage(WM_CLOSE);

	return 0;
}

LRESULT MainTabView::OnAdbPath(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = static_cast<BOOL>(wParam);
	if (bResult)
	{
		HandleAdbPathSuccess((LPCTSTR)lParam);
	}
	else
	{
		HandleAdbPathError((HWND)lParam);
	}
	return 0;
}

LRESULT MainTabView::OnPrepareAdb(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_dlgPreparing.IsShowing())
	{
		return 0;
	}
	if (m_ddmLibWrapper.IsInit())
	{
		return 0;
	}
	// wait MIN_SHOW_TIME to show preparing dialog
	SetTimer(INIT_TIMER_ID, MIN_SHOW_TIME);
	return 0;
}

LRESULT MainTabView::OnAdbPrepareFinish(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_bPrepareFinish = TRUE;
	if (m_dlgPreparing.IsShowing())
	{
		if (m_dwDlgStartTime > 0)
		{
			DWORD dwEndTime = ::GetTickCount();
			DWORD dwLen = dwEndTime - m_dwDlgStartTime;
			if (dwLen > MIN_DIALOG_TIME)
			{
				m_dlgPreparing.EndDialog(IDOK);
			}
			else
			{
				// wait dialog show time, at least MIN_DIALOG_TIME
				SetTimer(WAIT_TIMER_ID, MIN_DIALOG_TIME - dwLen);
			}
		}
		else
		{
			// error here
			m_dlgPreparing.EndDialog(IDOK);
		}
	}
	return 0;
}

void MainTabView::InitFinish()
{
	// adb init finish
	SendMessage(MSG_MAIN_ADB_FINISH);
}

void MainTabView::OnDeviceUpdated(const CSimpleArray<IDevice*>& arrDevice)
{
	GetParent().SendMessage(MSG_RIBBON_DEVICE_LIST, 0, (LPARAM) &arrDevice);
}

bool MainTabView::CreateTabControl()
{
	CTabViewImpl<MainTabView>::CreateTabControl();
	m_tab.ModifyStyle(0, TCS_BOTTOM);
	return true;
}

void MainTabView::UpdateLayout()
{
	RECT rect = { 0 };
	GetClientRect(&rect);

	if ((m_tab.GetStyle() & TCS_BOTTOM) != 0)
	{
		int cyOffset = 0;
		if (m_tab.IsWindow() && ((m_tab.GetStyle() & WS_VISIBLE) != 0))
		{
			cyOffset = m_cyTabHeight;
			m_tab.SetWindowPos(NULL, 0, rect.bottom - rect.top - cyOffset, rect.right - rect.left, m_cyTabHeight, SWP_NOZORDER);
		}

		if (m_nActivePage != -1)
			::SetWindowPos(GetPageHWND(m_nActivePage), NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top - cyOffset, SWP_NOZORDER);
	}
	else
	{
		CTabViewImpl<MainTabView>::UpdateLayout();
	}
}

void MainTabView::InitTabs()
{
	AddMainTab();
	AddSettingTab();
	SetActivePage(0);
}

void MainTabView::DestroyTabs()
{
	for (int i = 0; i < m_arrWnd.GetSize(); i++)
	{
		CWindow* wnd = m_arrWnd[i];
		if (wnd != NULL)
		{
			delete wnd;
		}
	}
	m_arrWnd.RemoveAll();
}

void MainTabView::AddMainTab()
{
	MainTab* mainView = new MainTab;
	mainView->Create(*this);
	m_arrWnd.Add(mainView);

	AddPage(*mainView, _T("Main"), TAB_MAIN);
}

void MainTabView::AddSettingTab()
{
	SettingTab* configView = new SettingTab;
	configView->Create(*this);
	m_arrWnd.Add(configView);

	AddPage(*configView, _T("Setting"), TAB_SETTING);
}

void MainTabView::HandleAdbPathError(HWND hWndSetting)
{
	if (m_dlgPreparing.IsShowing())
	{
		// cancel preparing dialog
		m_dlgPreparing.EndDialog(IDOK);
	}
	AdbPathErrorDlg dlg;
	int nRet = dlg.DoModal();
	if (nRet == IDOK)
	{
		::PostMessage(hWndSetting, MSG_SETTING_SELECT_ADB, 0, 0);
	}
	else if (nRet == IDCANCEL)
	{
		GetParent().PostMessage(WM_CLOSE);
	}
}

void MainTabView::HandleAdbPathSuccess(LPCTSTR lpszAdbPath)
{
	if (lpszAdbPath != NULL && _tcslen(lpszAdbPath) > 0)
	{
		m_ddmLibWrapper.SetDdmCallback(this);
		m_ddmLibWrapper.Init(lpszAdbPath);
	}
}
