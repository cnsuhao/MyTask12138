#if !defined(AFX_MYBUTTON_H__9FB3F34B_F78A_4C4B_89FB_8BBFDAB61181__INCLUDED_)
#define AFX_MYBUTTON_H__9FB3F34B_F78A_4C4B_89FB_8BBFDAB61181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CBitmapButton
{
// Construction
public:
	CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	CRgn m_rgn;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__9FB3F34B_F78A_4C4B_89FB_8BBFDAB61181__INCLUDED_)
