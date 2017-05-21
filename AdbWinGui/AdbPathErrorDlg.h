#pragma once

#include <atldlgs.h>

class AdbPathErrorDlg : public CTaskDialogImpl<AdbPathErrorDlg>
{
public:
	AdbPathErrorDlg();

    BOOL OnButtonClicked(int buttonId);
	int DoModal(HWND hWnd);

private:
	int m_nClickedId;
	enum
	{
		em_Button_Select = 101,
		em_Button_Exit
	};

};
