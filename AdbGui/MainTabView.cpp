// MainTabView.cpp: implement for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTabView.h"
#include "MainTab.h"
#include "ConfigTab.h"

BOOL MainTabView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

void MainTabView::InitTabs()
{
	AddMainTab();
	AddConfigTab();
}

void MainTabView::AddMainTab()
{
	MainTab* mainView = new MainTab;
	mainView->Create(*this, rcDefault, _T("MainTab"), WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);

	AddTab(_T("Main"), *mainView, TRUE, TAB_MAIN, (LPARAM)mainView);
}

void MainTabView::AddConfigTab()
{
	ConfigTab*	configView = new ConfigTab;
	configView->Create(*this, rcDefault, _T("ConfigTab"), WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);

	AddTab(_T("Config"), *configView, FALSE, TAB_CONFIG, (LPARAM)configView);
}

void MainTabView::OnTabRemoved(int inTabIndex)
{
	LPARAM theTabParam = GetTabParam(inTabIndex);
	if (theTabParam != 0)
	{
		int theTabImage = GetTabImage(inTabIndex);
		switch (theTabImage)
		{
		case TAB_MAIN:
		{
			MainTab* theWindowPtr = reinterpret_cast<MainTab*>(theTabParam);
			theWindowPtr->DestroyWindow();
			delete theWindowPtr;

			break;
		}
		case TAB_CONFIG:
		{
			ConfigTab* theWindowPtr = reinterpret_cast<ConfigTab*>(theTabParam);
			theWindowPtr->DestroyWindow();
			delete theWindowPtr;

			break;
		}
		}
	}
}