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

class FileExistsDlg : public CTaskDialogImpl<FileExistsDlg>
{
private:
	enum
	{
		em_Button_Replace = 101,
		em_Button_Cancel
	};
	CString m_strTarget;

public:
	FileExistsDlg(LPCTSTR lpszFrom, LPCTSTR lpszTo);

	BOOL OnButtonClicked(int buttonId)/* override */;
	INT DoModal(HWND hWnd = ::GetActiveWindow(), BOOL* pbChecked = NULL)/* override */;

private:
	INT m_nClickedId;
};