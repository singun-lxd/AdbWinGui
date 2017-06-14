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
#include "InstallNotifyDlg.h"
#include "resource.h"

InstallNotifyDlg::InstallNotifyDlg(LPCTSTR lpszApkPath)
{
	SetWindowTitle(IDR_MAINFRAME);
	SetMainInstructionText(IDS_INSTALL_APK_FILE_NOTICE);
	SetMainIcon(TD_INFORMATION_ICON);
	SetContentText(lpszApkPath);

	ModifyFlags(0, TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS |
		TDF_POSITION_RELATIVE_TO_WINDOW);
}

BOOL InstallNotifyDlg::OnButtonClicked(int buttonId)
{
	m_nClickedId = buttonId;
	return TRUE;
}

INT InstallNotifyDlg::DoModal()
{
	const TASKDIALOG_BUTTON buttons[] =
	{
		{ em_Button_Install_Direct, MAKEINTRESOURCE(IDS_INSTALL_APK) },
		{ em_Button_Install_With_Copy, MAKEINTRESOURCE(IDS_COPY_AND_INSTALL) },
		{ em_Button_Install_Cancel, MAKEINTRESOURCE(IDS_CANCEL) },
	};

	SetButtons(buttons, _countof(buttons));

	CTaskDialogImpl::DoModal();

	return m_nClickedId;
}
