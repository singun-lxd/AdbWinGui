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
#include <atlctrls.h>

template < class T, class TT = CToolTipCtrl >
class CDialogToolTip : public CMessageMap
{
private:
	TT m_ttTipControl;
	UINT m_uStyle;
	UINT m_uFlags;

protected:
	CDialogToolTip(UINT uTTSTyle = TTS_NOPREFIX | TTS_USEVISUALSTYLE,
		UINT uToolFlags = TTF_IDISHWND)
		: m_ttTipControl(NULL),
		m_uStyle(uTTSTyle),
		m_uFlags(uToolFlags | TTF_SUBCLASS | TTF_TRANSPARENT)
	{}

private:
	HWND GetHWND(UINT idTool)
	{
		return ::GetDlgItem(*(T*)this, idTool);
	}

	static BOOL CALLBACK SetControlToolTips(HWND hTool, LPARAM pDlg)
	{
		T* pT = (T*)pDlg;
		int idTool = ::GetWindowLong(hTool, GWL_ID);
		if (idTool != IDC_STATIC)
		{
			CToolInfo ToolInfo(pT->m_uFlags, hTool, 0, 0, (LPTSTR)idTool);
			pT->m_ttTipControl.AddTool(&ToolInfo);
		}
		return TRUE;
	}

public:
	BEGIN_MSG_MAP(CDialogToolTip)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouse)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		OnInitToolTip();
		bHandled = FALSE;
		return TRUE;
	}

	LRESULT OnMouse(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		T* pT = (T*)this;
		bHandled = FALSE;
		if (m_ttTipControl.IsWindow())
		{
			m_ttTipControl.RelayEvent((LPMSG)pT->GetCurrentMessage());
		}
		return 0;
	}

	void OnInitToolTip()
	{
		T* pThis = static_cast<T*>(this);
		ATLASSERT(::IsWindow(*pThis));

		m_ttTipControl.Create(*pThis, NULL, NULL, m_uStyle);
		CToolInfo ToolInfo(pThis->m_uFlags, *pThis, 0, 0, MAKEINTRESOURCE(pThis->IDD));
		m_ttTipControl.AddTool(&ToolInfo);
		::EnumChildWindows(*pThis, SetControlToolTips, (LPARAM)pThis);
		SetToolTipMaxWidth(0);
		ActivateToolTip(TRUE);
	}

	void ActivateToolTip(BOOL bActivate)
	{
		m_ttTipControl.Activate(bActivate);
	}

	void SetToolTipText(HWND hTool, _U_STRINGorID text)
	{
		m_ttTipControl.UpdateTipText(text, hTool);
	}

	void SetToolTipText(UINT idTool, _U_STRINGorID text)
	{
		SetToolTipText(GetHWND(idTool), text);
	}

	void SetToolTipMaxWidth(int nPixel)
	{
		m_ttTipControl.SetMaxTipWidth(nPixel);
	}

	BOOL AddToolTip(HWND hTool)
	{
		return SetControlToolTips(hTool, (LPARAM)(T*)this);
	}
	BOOL AddToolTip(UINT idTool)
	{
		return AddToolTip(GetHWND(idTool));
	}

	void RemoveToolTip(HWND hTool)
	{
		m_ttTipControl.DelTool(hTool);
	}

	void RemoveToolTip(UINT idTool)
	{
		RemoveToolTip(GetHWND(idTool));
	}
};