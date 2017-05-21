// MainTabView.cpp: implement for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainTabView.h"
#include "MainTab.h"
#include "SettingTab.h"

BOOL MainTabView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

void MainTabView::InitTabs()
{
	AddMainTab();
	AddSettingTab();
}

void MainTabView::AddMainTab()
{
	MainTab* mainView = new MainTab;
	mainView->Create(*this, rcDefault, _T("MainTab"), WS_CHILD, WS_EX_STATICEDGE);

	AddTab(_T("Main"), *mainView, TRUE, TAB_MAIN, (LPARAM)mainView);
}

void MainTabView::AddSettingTab()
{
	SettingTab*	configView = new SettingTab;
	configView->Create(*this);

	AddTab(_T("Setting"), *configView, FALSE, TAB_SETTING, (LPARAM)configView);
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
		case TAB_SETTING:
		{
			SettingTab* theWindowPtr = reinterpret_cast<SettingTab*>(theTabParam);
			theWindowPtr->DestroyWindow();
			delete theWindowPtr;

			break;
		}
		}
	}
}