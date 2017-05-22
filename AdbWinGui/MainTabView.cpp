// MainTabView.cpp: implement for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTabView.h"
#include "MainTab.h"
#include "SettingTab.h"
#include "AdbPathErrorDlg.h"
#include "MessageDefine.h"

BOOL MainTabView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT MainTabView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CTabViewImpl<MainTabView>::OnDestroy(uMsg, wParam, lParam, bHandled);
	DestroyTabs();
	return 0;
}

LRESULT MainTabView::OnNotifyExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	GetParent().PostMessage(WM_CLOSE);

	return 0;
}

LRESULT MainTabView::OnAdbError(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == MSG_MAIN_ADB_ERROR)
	{
		AdbPathErrorDlg dlg;
		int nRet = dlg.DoModal(GetParent());
		if (nRet == IDOK)
		{
			HWND hWndSetting = (HWND)lParam;
			::PostMessage(hWndSetting, MSG_SETTING_SELECT_ADB, 0, 0);
		}
		else
		{
			GetParent().PostMessage(WM_CLOSE);
		}
	}
	return 0;
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
	mainView->Create(*this, rcDefault, _T("MainTab"), WS_CHILD, WS_EX_STATICEDGE);
	m_arrWnd.Add(mainView);

	AddPage(*mainView, _T("Main"), TAB_MAIN);

	//AddTab(_T("Main"), *mainView, TRUE, TAB_MAIN, (LPARAM)mainView);
}

void MainTabView::AddSettingTab()
{
	SettingTab* configView = new SettingTab;
	configView->Create(*this);
	m_arrWnd.Add(configView);

	AddPage(*configView, _T("Setting"), TAB_SETTING);
	//AddTab(_T("Setting"), *configView, FALSE, TAB_SETTING, (LPARAM)configView);
}

void MainTabView::CalcViewRect(CRect* pRect)
{
	const LONG TABVIEW_BORDER = 3;
	const LONG TABVIEW_EDGE = 5;

	GetClientRect((*pRect));

	if (pRect->Height() > 0 && pRect->Width() > 0)
	{
		// Calculate the Height (or Width) of the tab . . .
		// cause it could be Multiline
		CRect theTabRect;
		m_tab.GetItemRect(0, &theTabRect);

		LONG theRowCount = m_tab.GetRowCount();
		LONG theEdgeWidth = (theTabRect.Width() * theRowCount) + TABVIEW_EDGE;
		LONG theEdgeHeight = (theTabRect.Height() * theRowCount) + TABVIEW_EDGE;

		// Set the size based on the style
		DWORD dwStyle = GetStyle();
		if ((dwStyle & TCS_BOTTOM) && !(dwStyle & TCS_VERTICAL)) {		// Bottom
			(*pRect).top += TABVIEW_BORDER;
			(*pRect).left += TABVIEW_BORDER;
			(*pRect).right -= TABVIEW_BORDER;
			(*pRect).bottom -= theEdgeHeight;
		}
		else if ((dwStyle & TCS_RIGHT) && (dwStyle & TCS_VERTICAL)) {	// Right
			(*pRect).top += TABVIEW_BORDER;
			(*pRect).left += TABVIEW_BORDER;
			(*pRect).right -= theEdgeWidth;
			(*pRect).bottom -= TABVIEW_BORDER;
		}
		else if (dwStyle & TCS_VERTICAL) {								// Left
			(*pRect).top += TABVIEW_BORDER;
			(*pRect).left += theEdgeWidth;
			(*pRect).right -= TABVIEW_BORDER;
			(*pRect).bottom -= TABVIEW_BORDER;
		}
		else {															// Top
			(*pRect).top += theEdgeHeight;
			(*pRect).left += TABVIEW_BORDER;
			(*pRect).right -= TABVIEW_BORDER;
			(*pRect).bottom -= TABVIEW_BORDER;
		}
	}
}