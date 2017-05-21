// SettingTab.h : interface of the SettingTab class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>
#include "resource.h"
#include "Config/ConfigManager.h"
#include "MessageDefine.h"

class SettingTab : public CDialogImpl<SettingTab>
{
public:
	enum {
		IDD = IDD_SETTING_TAB
	};

	CButton m_btnRadioAuto;
	CButton m_btnRadioManual;
	CStatic m_stcAdbPath;
	CEdit m_edtAdbPath;

public:
	BEGIN_MSG_MAP(SettingTab)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(MSG_SETTING_SELECT_ADB, OnShowSelectAdbDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO_AUTO, IDC_RADIO_MANUAL, OnRadioSelected)
		//COMMAND_ID_HANDLER(IDC_EDIT_PATH, OnPathClick)
	END_MSG_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowSelectAdbDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRadioSelected(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnPathClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	void InitControls();
	void UpdateControlStatus();
	void CheckSettingValid();
	void SwitchToAutoMode(LPCTSTR lpszePath);
	void SwitchToManualMode(LPCTSTR lpszePath);
	void SwitchRadioButton(BOOL bAuto);
	void AutoUpdateAdbPath(ConfigManager& cfgManager);
	BOOL ShowSelectAdbDialog(ConfigManager& cfgManager);
};
