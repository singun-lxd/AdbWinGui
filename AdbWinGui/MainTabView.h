// MainTabView.h: interface for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "TabViewCtrl.h"

class MainTabView : public CTabViewCtrl
{
protected:
	enum
	{
		TAB_MAIN = 1,
		TAB_SETTING = 2,
	};

public:
	DECLARE_WND_SUPERCLASS(NULL, CTabViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP_EX(MainTabView)
		CHAIN_MSG_MAP(CTabViewCtrl)
	END_MSG_MAP()

public:
	void InitTabs();

protected:
	void AddMainTab();
	void AddSettingTab();
	virtual void OnTabRemoved(int inTabIndex);
};
