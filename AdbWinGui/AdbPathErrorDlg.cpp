#include "stdafx.h"
#include "AdbPathErrorDlg.h"
#include "resource.h"


AdbPathErrorDlg::AdbPathErrorDlg()
{
	SetWindowTitle(IDR_MAINFRAME);
	SetMainInstructionText(IDS_ADB_PATH_ERROR);
	SetMainIcon(TD_ERROR_ICON);

	ModifyFlags(0, TDF_ALLOW_DIALOG_CANCELLATION | 
		TDF_USE_COMMAND_LINKS);
}

void AdbPathErrorDlg::OnDialogConstructed()
{
	SetButtonElevationRequiredState(em_Button_Select, true);
}

BOOL AdbPathErrorDlg::OnButtonClicked(int buttonId)
{
	switch (buttonId)
	{
	case em_Button_Select:
		m_nClickedId = IDOK;
		break;
	case em_Button_Exit:
		m_nClickedId = IDCANCEL;
		break;
	}

	return TRUE;
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

	CTaskDialogImpl::DoModal(hWnd);

	return m_nClickedId;
}

