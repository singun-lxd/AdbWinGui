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
#include "resource.h"
#include "AdbPreparingDlg.h"

AdbPreparingDlg::AdbPreparingDlg()
{
	SetWindowTitle(IDR_MAINFRAME);
	SetMainInstructionText(IDS_ADB_PREPARING);
	//SetMainIcon(TD_INFORMATION_ICON);
	SetCommonButtons(TDCBF_CANCEL_BUTTON);

	ModifyFlags(0, TDF_ALLOW_DIALOG_CANCELLATION |
		TDF_SHOW_MARQUEE_PROGRESS_BAR | TDF_POSITION_RELATIVE_TO_WINDOW);
}

void AdbPreparingDlg::OnCreated()
{
	SetProgressBarMarquee(TRUE, 1);
}

BOOL AdbPreparingDlg::OnButtonClicked(int buttonId)
{
	m_nReturn = buttonId;
	return TRUE;
}

INT AdbPreparingDlg::DoModal(HWND hWnd)
{
	CTaskDialogImpl::DoModal(hWnd);
	return m_nReturn;
}

BOOL AdbPreparingDlg::EndDialog(int nRetCode)
{
	m_nReturn = nRetCode;
	return ::EndDialog(this->m_hWnd, nRetCode);
}

BOOL AdbPreparingDlg::IsShowing()
{
	return m_hWnd != NULL;
}

