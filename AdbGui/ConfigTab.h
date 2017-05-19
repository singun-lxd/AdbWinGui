// ConfigTab.h : interface of the ConnfigTab class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>

class ConfigTab : public CWindowImpl<ConfigTab, CListViewCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(ConfigTab)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};
