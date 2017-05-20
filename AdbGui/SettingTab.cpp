// ConfigTab.cpp : implement of the ConnfigTab class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SettingTab.h"

BOOL SettingTab::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}