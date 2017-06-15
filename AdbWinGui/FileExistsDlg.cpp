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
#include "FileExistsDlg.h"
#include "resource.h"

FileExistsDlg::FileExistsDlg(LPCTSTR lpszFrom, LPCTSTR lpszTo)
{
	SetWindowTitle(IDR_MAINFRAME);
	SetMainInstructionText(IDS_FILE_EXISTS);
	SetMainIcon(TD_WARNING_ICON);
	SetContentText(lpszTo);
	m_strTarget = lpszFrom;

	ModifyFlags(0, TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS |
		TDF_POSITION_RELATIVE_TO_WINDOW);
}

BOOL FileExistsDlg::OnButtonClicked(int buttonId)
{
	switch (buttonId)
	{
	case em_Button_Replace:
		m_nClickedId = IDOK;
		break;
	default:
		m_nClickedId = IDCANCEL;
		break;
	}

	return TRUE;
}

INT FileExistsDlg::DoModal(HWND hWnd, BOOL* pbChecked)
{
	CString strButtonReplace;
	strButtonReplace.LoadString(IDS_REPLACE_FILE);
	strButtonReplace.AppendFormat(_T("\n%s"), m_strTarget);

	const TASKDIALOG_BUTTON buttons[] =
	{
		{ em_Button_Replace, strButtonReplace },
		{ em_Button_Cancel, MAKEINTRESOURCE(IDS_CANCEL) },
	};

	SetButtons(buttons, _countof(buttons));

	if (pbChecked != NULL)
	{
		ModifyFlags(TDF_VERIFICATION_FLAG_CHECKED, 0);
		SetVerificationText(IDS_REMEMBER_MY_CHOICE);
	}
	CTaskDialogImpl::DoModal(hWnd, NULL, NULL, pbChecked);

	return m_nClickedId;
}
