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

// MainFrm.cpp : implemennt of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "resource.h"

#define MIN_WINDOW_WIDTH		640
#define MIN_WINDOW_HEIGHT	480

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (CRibbonFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_MainTabView.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnQueryEndSession(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// todo Save data

	return TRUE; // allow automatic close
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

	m_hWndClient = m_MainTabView.Create(m_hWnd, rcDefault, _T("MainTabView"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
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

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	MINMAXINFO* lpMMI = (MINMAXINFO*) lParam;
	// set min window size
	lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;

	return 0;
}

LRESULT CMainFrame::OnDeviceUpdate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
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

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewRibbon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowRibbonUI(!IsRibbonUI());
	UISetCheck(ID_VIEW_RIBBON, IsRibbonUI());
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void CMainFrame::InitRibbonUI()
{
	HBITMAP hbm = DefRibbonQueryImage(IDB_DEVICE);
	m_bmpDevice.Attach(hbm);
	UpdateNoneDevice();
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

	m_glyDevices.InvalidateItems();

	if (nSize == 1)
	{
		m_glyDevices.Select(0, true);
	}
}
