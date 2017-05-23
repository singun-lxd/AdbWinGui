/*
AdbWinGui (Android Debug Bridge Windows GUI)
Copyright (C) 2017  singun

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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