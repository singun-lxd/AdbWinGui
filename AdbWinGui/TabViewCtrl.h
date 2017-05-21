// TabViewCtrl based on WTL

#pragma once

#include <atlwin.h>
#include <atlctrls.h>
#include <atlgdi.h>
#include <atlcrack.h>
#include <atlmisc.h>

class CTabViewCtrl : public CWindowImpl< CTabViewCtrl, CTabCtrl >
{
private:

	const LONG TABVIEW_BORDER;			///< Border Width
	const LONG TABVIEW_EDGE;			///< Distance of tab from content

private:

	CSimpleArray<CWindow>	m_Views;			///< An array of views for the tab
	LONG						m_ActiveTabIndex;	///< The index of the active tab
	CWindow					m_ActiveTabWindow;	///< The active tab window
	CFont						m_HorizFont;		///< Top/Bottom font used by tab control
	CFont						m_LeftFont;			///< Left font used by tab control
	CFont						m_RightFont;		///< Right font used by tab control

public:

	CTabViewCtrl() :
		TABVIEW_BORDER(3),
		TABVIEW_EDGE(5),
		m_ActiveTabIndex(-1)
	{
	}

	virtual ~CTabViewCtrl()
	{
	}

public:

	BOOL AddTab(LPCTSTR inLabel, HWND inTabWindow, BOOL inActiveFlag = TRUE, int inImage = -1, LPARAM inParam = 0)
	{
		BOOL	theReturn;
		CWindow theTabWindow(inTabWindow);

		// Make sure it's a real window
		ATLASSERT(theTabWindow.IsWindow());

		// Make sure it's a child window and is not visible
		ATLASSERT((theTabWindow.GetStyle() & WS_CHILD) != 0);
		ATLASSERT((theTabWindow.GetStyle() & WS_VISIBLE) == 0);

		// Hide the view window
		theTabWindow.EnableWindow(FALSE);
		theTabWindow.ShowWindow(SW_HIDE);

		// Store the required data for the list
		m_Views.Add(theTabWindow);

		// Add the tab to the tab control
		TC_ITEM theItem = { 0 };
		theItem.mask = TCIF_TEXT;
		theItem.pszText = const_cast<TCHAR*>(inLabel);

		// Add an image for the tab
		if (inImage != -1)
		{
			theItem.mask |= TCIF_IMAGE;
			theItem.iImage = inImage;
		}

		// Add the param for the tab
		if (inParam != 0)
		{
			theItem.mask |= TCIF_PARAM;
			theItem.lParam = inParam;
		}

		// Insert the item at the end of the tab control
		theReturn = InsertItem(32768, &theItem);

		// Set the position for the window
		CRect rcChild;
		CalcViewRect(&rcChild);
		theTabWindow.MoveWindow(rcChild);

		// Select the tab that is being added, if desired
		LONG theTabIndex = GetTabCount() - 1;
		if (inActiveFlag || theTabIndex == 0)
		{
			SetActiveTab(theTabIndex);
		}

		return theReturn;
	}

	LONG GetTabCount()
	{
		return m_Views.GetSize();
	}

	HWND GetTab(int inTab)
	{
		HWND theTabHwnd = NULL;

		if (inTab >= 0 && inTab < GetTabCount())
		{
			m_Views[inTab];
		}

		return theTabHwnd;
	}

	virtual void OnTabRemoved(int inTabIndex)
	{
		UNREFERENCED_PARAMETER(inTabIndex);
	}

	HWND RemoveTab(int inTab)
	{
		HWND theTabHwnd = NULL;
		LONG theNewTab = -1;

		if (inTab >= 0 && inTab < GetTabCount())
		{
			// Select a new tab if the tab is active
			if (m_ActiveTabIndex == inTab)
			{
				m_ActiveTabIndex = -1;
				m_ActiveTabWindow = NULL;

				if (GetTabCount() > 1)
				{
					theNewTab = (inTab > 0) ? (inTab - 1) : 0;
				}
			}

			// Save the window that is begin removed
			theTabHwnd = m_Views[inTab];

			// Virtual method to notify subclasses that a tab was removed
			OnTabRemoved(inTab);

			// Remove the item from the view list
			m_Views.RemoveAt(inTab);

			// Remove the tab
			if (IsWindow())
			{
				DeleteItem(inTab);
			}

			SetActiveTab(theNewTab);
		}

		return theTabHwnd;
	}

	void RemoveAllTabs()
	{
		LONG theCount = GetTabCount();
		for (LONG theIndex = theCount - 1; theIndex >= 0; theIndex--)
		{
			RemoveTab(theIndex);
		}
	}

	void SetActiveTab(int inNewTab)
	{
		if (inNewTab >= 0 && inNewTab < GetTabCount() &&	// Validate the tab index range
			inNewTab != m_ActiveTabIndex)					// Don't select if already selected
		{
			// Disable the old tab
			if (m_ActiveTabWindow.IsWindow())
			{
				m_ActiveTabWindow.EnableWindow(FALSE);
				m_ActiveTabWindow.ShowWindow(SW_HIDE);
			}

			// Enable the new tab
			m_ActiveTabWindow = m_Views[inNewTab];

			m_ActiveTabWindow.EnableWindow(TRUE);
			m_ActiveTabWindow.ShowWindow(SW_SHOW);
			m_ActiveTabWindow.SetFocus();

			m_ActiveTabWindow.Invalidate(TRUE);

			m_ActiveTabIndex = inNewTab;

			// Select the tab (if tab programmatically changed)
			SetCurSel(m_ActiveTabIndex);
		}
	}

	HWND GetActiveTab()
	{
		return GetTab(m_ActiveTabIndex);
	}

	LONG GetActiveTabIndex()
	{
		return m_ActiveTabIndex;
	}

	void GetTabText(int inTab, int inSize, TCHAR* outText)
	{
		if (inTab >= 0 && inTab < GetTabCount())
		{
			// Get tab item info
			TCITEM tci;
			tci.mask = TCIF_TEXT;
			tci.pszText = outText;
			tci.cchTextMax = inSize;
			GetItem(inTab, &tci);
		}
	}

#ifdef __ATLSTR_H__

	const CString GetTabText(int inTab)
	{
		CString theTabText;

		if (inTab >= 0 && inTab < GetTabCount())
		{
			// Get tab item info
			TCHAR theText[MAX_PATH];

			GetTabText(inTab, sizeof(theText), theText);
			theTabText = theText;
		}

		return theTabText;
	}

#endif

	LPARAM GetTabParam(int inTab)
	{
		TCITEM tci = { 0 };
		if (inTab >= 0 && inTab < GetTabCount())
		{
			// Get tab item info
			tci.mask = TCIF_PARAM;
			GetItem(inTab, &tci);
		}

		return tci.lParam;
	}

	int GetTabImage(int inTab)
	{
		TCITEM tci = { 0 };
		if (inTab >= 0 && inTab < GetTabCount())
		{
			// Get tab item info
			tci.mask = TCIF_IMAGE;
			GetItem(inTab, &tci);
		}

		return tci.iImage;
	}

	BOOL ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
	{
		// Modify the style
		BOOL theReturn = ModifyStyle(dwRemove, dwAdd, nFlags);

		// Update all the views in case the window positions changes
		UpdateViews();

		// Set the font in case the tab position changed
		SetTabFont(dwAdd);

		return theReturn;
	}

public:

	BEGIN_MSG_MAP_EX(CTabViewCtrl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(TCN_SELCHANGE, OnSelectionChanged)
	END_MSG_MAP()


protected:

	void UpdateViews()
	{
		CRect rcChild;
		CalcViewRect(&rcChild);

		LONG theCount = GetTabCount();
		for (LONG theIndex = 0; theIndex < theCount; theIndex++)
		{
			m_Views[theIndex].MoveWindow(rcChild);
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

		// Get the log font.
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

		// Top and Bottom Tab Font
		m_HorizFont.CreateFontIndirect(&ncm.lfMessageFont);

		// Left Tab Font
		ncm.lfMessageFont.lfOrientation = 900;
		ncm.lfMessageFont.lfEscapement = 900;
		m_LeftFont.CreateFontIndirect(&ncm.lfMessageFont);

		// Right Tab Font	
		ncm.lfMessageFont.lfOrientation = 2700;
		ncm.lfMessageFont.lfEscapement = 2700;
		m_RightFont.CreateFontIndirect(&ncm.lfMessageFont);

		// Check styles to determine which font to set
		SetTabFont(GetStyle());

		return lRet;
	}

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		RemoveAllTabs();

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnWindowPosChanged(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		UpdateViews();

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnSelectionChanged(LPNMHDR)
	{
		SetActiveTab(GetCurSel());

		return 0;
	}

protected:

	void CalcViewRect(CRect* pRect)
	{
		GetClientRect((*pRect));

		if (pRect->Height() > 0 && pRect->Width() > 0)
		{
			// Calculate the Height (or Width) of the tab . . .
			// cause it could be Multiline
			CRect theTabRect;
			GetItemRect(0, &theTabRect);

			LONG theRowCount = GetRowCount();
			LONG theEdgeWidth = (theTabRect.Width() * theRowCount) + TABVIEW_EDGE;
			LONG theEdgeHeight = (theTabRect.Height() * theRowCount) + TABVIEW_EDGE;

			// Set the size based on the style
			DWORD dwStyle = GetStyle();
			if ((dwStyle & TCS_BOTTOM) && !(dwStyle & TCS_VERTICAL)) {		// Bottom
				(*pRect).top += TABVIEW_BORDER;
				(*pRect).left += TABVIEW_BORDER;
				(*pRect).right -= TABVIEW_BORDER;
				(*pRect).bottom -= theEdgeHeight;
			}
			else if ((dwStyle & TCS_RIGHT) && (dwStyle & TCS_VERTICAL)) {	// Right
				(*pRect).top += TABVIEW_BORDER;
				(*pRect).left += TABVIEW_BORDER;
				(*pRect).right -= theEdgeWidth;
				(*pRect).bottom -= TABVIEW_BORDER;
			}
			else if (dwStyle & TCS_VERTICAL) {								// Left
				(*pRect).top += TABVIEW_BORDER;
				(*pRect).left += theEdgeWidth;
				(*pRect).right -= TABVIEW_BORDER;
				(*pRect).bottom -= TABVIEW_BORDER;
			}
			else {															// Top
				(*pRect).top += theEdgeHeight;
				(*pRect).left += TABVIEW_BORDER;
				(*pRect).right -= TABVIEW_BORDER;
				(*pRect).bottom -= TABVIEW_BORDER;
			}
		}
	}

	void SetTabFont(DWORD inStyleBits)
	{
		if (inStyleBits & TCS_VERTICAL)
		{
			if (inStyleBits & TCS_RIGHT)
			{
				SetFont(m_RightFont);
			}

			else
			{
				SetFont(m_LeftFont);
			}
		}

		else
		{
			SetFont(m_HorizFont);
		}
	}
};

