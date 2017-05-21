#pragma once

#include <atldlgs.h>
#include <windef.h>

class MessageTaskDlg : public CTaskDialogImpl<MessageTaskDlg>
{
public:
	MessageTaskDlg();

	BOOL OnButtonClicked(int buttonId);
	void OnVerificationClicked(bool bChecked);
	int DoModal(HWND hWnd, LPCTSTR lpszText, UINT nType, BOOL* pbChecked = NULL);

protected:
	inline void SetButton(UINT nType);
	inline void SetIcon(UINT nType);

protected:
	int m_nReturn;
	BOOL m_bNotAsk;
	CString m_strTitle;
};