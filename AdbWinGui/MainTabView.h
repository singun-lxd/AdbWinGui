// MainTabView.h: interface for the MainTabView class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include "resource.h"
#include "MessageDefine.h"

class MainTabView : public CTabViewImpl<MainTabView>
{
protected:
	enum
	{
		TAB_MAIN = 1,
		TAB_SETTING = 2,
	};
	CSimpleArray<CWindow*> m_arrWnd;

public:
	DECLARE_WND_SUPERCLASS(NULL, CTabViewImpl<MainTabView>::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP_EX(MainTabView)
		CHAIN_MSG_MAP(CTabViewImpl<MainTabView>)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(MSG_MAIN_NOTIFY_EXIT, OnNotifyExit)
		MESSAGE_HANDLER(MSG_MAIN_ADB_ERROR, OnAdbError)
	ALT_MSG_MAP(1)   // tab control
	END_MSG_MAP()

public:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNotifyExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAdbError(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	bool CreateTabControl();
	void UpdateLayout();
	void InitTabs();
	void DestroyTabs();

protected:
	void AddMainTab();
	void AddSettingTab();
};
