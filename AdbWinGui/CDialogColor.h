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

#include <atlwin.h>

template <class T>
class CDialogColor : public CMessageMap
{
private:
	CBrush m_brushBkgnd;

public:
	CDialogColor(COLORREF clrBg= RGB(255, 255, 255))
	{
		m_brushBkgnd.CreateSolidBrush(clrBg);
	}

	~CDialogColor()
	{
		if (m_brushBkgnd.m_hBrush != NULL)
		{
			m_brushBkgnd.DeleteObject();
		}
	}

	BEGIN_MSG_MAP(CDialogColor)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLORMSGBOX, OnCtlColorDialog)
		MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorDialog)
	END_MSG_MAP()

protected:
	HBRUSH CreateSolidBrush(COLORREF crColor)
	{
		if (m_brushBkgnd.m_hBrush != NULL)
		{
			m_brushBkgnd.DeleteObject();
		}
		m_brushBkgnd.CreateSolidBrush(crColor);
		return m_brushBkgnd.m_hBrush;
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		T* pThis = static_cast<T*>(this);
		HDC hDc = (HDC)wParam;
		RECT rcRect;
		pThis->GetClientRect(&rcRect);
		FillRect(hDc, &rcRect, m_brushBkgnd);
		return TRUE;
	}

	LRESULT OnCtlColorDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		HDC hDc = (HDC)wParam;
		switch (uMsg)
		{
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORLISTBOX:
		{
			::SetBkMode(hDc, TRANSPARENT);
			bHandled = TRUE;
			return (LRESULT)m_brushBkgnd.m_hBrush;
		}
		break;
		case WM_CTLCOLORDLG:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSCROLLBAR:
		default:
			break;
		}
		return NULL;
	}
};