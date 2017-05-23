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

#include "stdafx.h"
#include "AdbPathErrorDlg.h"
#include "resource.h"


AdbPathErrorDlg::AdbPathErrorDlg()
{
	SetWindowTitle(IDR_MAINFRAME);
	SetMainInstructionText(IDS_ADB_PATH_ERROR);
	SetMainIcon(TD_ERROR_ICON);

	ModifyFlags(0, TDF_ALLOW_DIALOG_CANCELLATION | 
		TDF_USE_COMMAND_LINKS | TDF_ENABLE_HYPERLINKS);
}

BOOL AdbPathErrorDlg::OnButtonClicked(int buttonId)
{
	switch (buttonId)
	{
	case em_Button_Select:
		m_nClickedId = IDOK;
		break;
	default:
		m_nClickedId = IDCANCEL;
		break;
	}

	return TRUE;
}

void AdbPathErrorDlg::OnHyperlinkClicked(LPCTSTR pszHREF)
{
	::ShellExecute(NULL, _T("open"), pszHREF, NULL, NULL, SW_SHOWNORMAL);
}

int AdbPathErrorDlg::DoModal(HWND hWnd)
{
	CString strContent;
	strContent.LoadString(IDS_ADB_PATH_INVALID);
	SetContentText(strContent);
	CString strCmdSelect, strCmdExit;
	strCmdSelect.LoadString(IDS_SELECT_ADB_PATH);
	strCmdExit.LoadString(IDS_EXIT_ADBWINGUI);

	const TASKDIALOG_BUTTON buttons[] =
	{
		{ em_Button_Select, strCmdSelect },
		{ em_Button_Exit, strCmdExit },
	};

	SetButtons(buttons, _countof(buttons));

	CString strFooter;
	strFooter.LoadString(IDS_DOWNLOAD_ADB);
	SetFooterText(strFooter);
	SetFooterIcon(TD_INFORMATION_ICON);

	CTaskDialogImpl::DoModal(hWnd);

	return m_nClickedId;
}

