#if !defined(AFX_MYSUBMITDLG_H__69A30DCA_5DDD_4381_ADE7_36F6089EFD0F__INCLUDED_)
#define AFX_MYSUBMITDLG_H__69A30DCA_5DDD_4381_ADE7_36F6089EFD0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyButton.h"
// MySubmitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySubmitDlg dialog

class CMySubmitDlg : public CDialog
{
// Construction
public:
	CString m_stTitle;
	int GetPercent();
	CMySubmitDlg(int tlastper, const char* name, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMySubmitDlg)
	enum { IDD = IDD_DIALOG_SUBMIT };
	CMyButton	m_btnOK;
	CMyButton	m_btnCancel;
	CComboBox	m_ctlPerChose;
	CString	m_stPer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySubmitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nLastPer;

	// Generated message map functions
	//{{AFX_MSG(CMySubmitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownComboPer();
	afx_msg void OnSelendcancelComboPer();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSUBMITDLG_H__69A30DCA_5DDD_4381_ADE7_36F6089EFD0F__INCLUDED_)
