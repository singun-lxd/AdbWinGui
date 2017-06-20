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

// MainFrm.cpp : implement of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "resource.h"

#pragma comment(lib, "UxTheme.lib")

#define MIN_WINDOW_WIDTH		640
#define MIN_WINDOW_HEIGHT	480

#define	 MIN_REFRESH_TIME		500
#define	 REFRESH_TIMER_ID		0x10

#define SOFT_REG_KEY			_T("Software\\singun\\AdbWiGui")

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (CRibbonFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
	{
		return TRUE;
	}

	return m_MainTabView.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

BOOL CMainFrame::OnQueryEndSession(UINT nSource, UINT uLogOff)
{
	// todo Save data

	return TRUE; // allow automatic close
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	bool bRibbonUI = RunTimeHelper::IsRibbonUIAvailable();
	if (bRibbonUI)
		UIAddMenu(GetMenu(), true);
	else
		CMenuHandle(GetMenu()).DeleteMenu(ID_VIEW_RIBBON, MF_BYCOMMAND);

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_MainTabView.Create(m_hWnd, rcDefault, _T("MainTabView"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_MainTabView.InitTabs();

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	ShowRibbonUI(bRibbonUI);
	UISetCheck(ID_VIEW_RIBBON, bRibbonUI);
	InitRibbonUI();

	return 0;
}

void CMainFrame::OnDestroy()
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	SetMsgHandled(FALSE);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case REFRESH_TIMER_ID:
		KillTimer(REFRESH_TIMER_ID);
		int nSel = m_glyDevices.GetSelected();
		m_glyDevices.Select(nSel, true);
		break;
	}
}

void CMainFrame::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
	// set min window size
	lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
}

LRESULT CMainFrame::OnDeviceUpdate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam)
{
	const CSimpleArray<IDevice*>* pArrDevice = (CSimpleArray<IDevice*>*) lParam;
	if (pArrDevice != NULL)
	{
		int nSize = pArrDevice->GetSize();
		if (nSize == 0)
		{
			UpdateNoneDevice();
		}
		else
		{
			UpdateDeviceList(*pArrDevice);
		}
	}
	return 0;
}

LRESULT CMainFrame::OnInstallApk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	LPCTSTR lpszApkPath = (LPCTSTR) lParam;
	m_recItems.AddToList(lpszApkPath);
	m_recItems.WriteToRegistry(SOFT_REG_KEY);
	return 0;
}

void CMainFrame::OnFileExit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	// TODO: add code to initialize document

}

void CMainFrame::OnViewToolBar(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
}

void CMainFrame::OnViewStatusBar(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
}

void CMainFrame::OnViewRibbon(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	ShowRibbonUI(!IsRibbonUI());
	UISetCheck(ID_VIEW_RIBBON, IsRibbonUI());
}

void CMainFrame::OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CAboutDlg dlg;
	dlg.DoModal(m_hWnd);
}

void CMainFrame::InitRibbonUI()
{
	m_recItems.ReadFromRegistry(SOFT_REG_KEY);
	HBITMAP hbm = DefRibbonQueryImage(IDB_DEVICE);
	m_bmpDevice.Attach(hbm);
	UpdateNoneDevice();
}

void CMainFrame::OnRibbonApkRecent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	// get file name from the MRU list
	TCHAR szFile[MAX_PATH];
	if (m_recItems.GetFromList(nID, szFile, MAX_PATH))
	{
		// install apk
		m_MainTabView.InstallApk(szFile);
		m_recItems.MoveToTop(nID);
		m_recItems.WriteToRegistry(SOFT_REG_KEY);
	}
	else
	{
		::MessageBeep(MB_ICONERROR);
	}
}

LRESULT CMainFrame::OnRibbonGalleryCtrl(UI_EXECUTIONVERB verb, WORD wID, UINT uSel, BOOL& bHandled)
{
	return 0;
}

void CMainFrame::UpdateNoneDevice()
{
	CString strError;
	strError.LoadString(IDS_DEVICE_EMPTY);
	m_glyDevices.Resize(1);
	m_glyDevices.SetItemImage(0, m_bmpDevice);
	m_glyDevices.SetItemText(0, strError);
	m_glyDevices.Select(0);
	m_glyDevices.InvalidateItems();
}

void CMainFrame::UpdateDeviceList(const CSimpleArray<IDevice*>& arrDevice)
{
	int nSize = arrDevice.GetSize();
	if (nSize > MAX_UI_DEVICE_LIST_COUNNT)
	{
		nSize = MAX_UI_DEVICE_LIST_COUNNT;
	}
	m_glyDevices.Resize(nSize);
	for (int i = 0; i < nSize; i++)
	{
		m_glyDevices.SetItemImage(i, m_bmpDevice);
		m_glyDevices.SetItemText(i, arrDevice[i]->GetSerialNumber());
	}
	if (nSize == 1)
	{
		m_glyDevices.Select(0);
	}
	m_glyDevices.InvalidateItems();

	// wait for invalidate event and select item
	SetTimer(REFRESH_TIMER_ID, MIN_REFRESH_TIME);
}
