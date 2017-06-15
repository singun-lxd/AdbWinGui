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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlribbon.h>
#include <atlcrack.h>
#include "resource.h"
#include "ribbon.h"
#include "MainTabView.h"
#include "MessageDefine.h"

#define MAX_UI_DEVICE_LIST_COUNNT	10

class CMainFrame : 
	public CRibbonFrameWindowImpl<CMainFrame>, 
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	MainTabView m_MainTabView;
	CCommandBarCtrl m_CmdBar;

	// Declare ribbon controls
	CRibbonItemGalleryCtrl<ID_VIEW_DEVICES, MAX_UI_DEVICE_LIST_COUNNT> m_glyDevices;
	CBitmap m_bmpDevice;

	// Ribbon control map
	BEGIN_RIBBON_CONTROL_MAP(CMainFrame)
		//RIBBON_CONTROL(m_btnCopy)
		RIBBON_CONTROL(m_glyDevices)
	END_RIBBON_CONTROL_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MSG_WM_QUERYENDSESSION(OnQueryEndSession)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MESSAGE_HANDLER_EX(MSG_RIBBON_DEVICE_LIST, OnDeviceUpdate)
		COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER_EX(ID_VIEW_RIBBON, OnViewRibbon)
		COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
		RIBBON_GALLERY_CONTROL_HANDLER(ID_VIEW_DEVICES, OnRibbonGalleryCtrl)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CRibbonFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	BOOL OnQueryEndSession(UINT nSource, UINT uLogOff);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
	LRESULT OnDeviceUpdate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

	void OnFileExit(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnFileNew(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnViewToolBar(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnViewStatusBar(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnViewRibbon(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnAppAbout(UINT uNotifyCode, int nID, CWindow wndCtl);

	void InitRibbonUI();
	LRESULT OnRibbonGalleryCtrl(UI_EXECUTIONVERB verb, WORD wID, UINT uSel, BOOL& bHandled);

private:
	void UpdateNoneDevice();
	void UpdateDeviceList(const CSimpleArray<IDevice*>& arrDevice);
};
