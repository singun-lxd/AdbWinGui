// ConfigTab.h : interface of the ConnfigTab class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>
#include "resource.h"

class SettingTab : public CDialogImpl<SettingTab>
{
public:
	enum {
		IDD = IDD_SETTING_TAB
	};

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(SettingTab)
	END_MSG_MAP()
};
