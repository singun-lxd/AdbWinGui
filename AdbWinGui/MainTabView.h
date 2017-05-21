// MainTabView.h: interface for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "TabViewCtrl.h"
#include "MessageDefine.h"

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
		MESSAGE_HANDLER(MSG_MAIN_NOTIFY_EXIT, OnNotifyExit)
		MESSAGE_HANDLER(MSG_MAIN_ADB_ERROR, OnAdbError)
		CHAIN_MSG_MAP(CTabViewCtrl)
	END_MSG_MAP()

public:
	LRESULT OnNotifyExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAdbError(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	void InitTabs();

protected:
	void AddMainTab();
	void AddSettingTab();
	virtual void OnTabRemoved(int inTabIndex);
};
