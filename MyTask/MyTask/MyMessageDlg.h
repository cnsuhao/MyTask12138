#if !defined(AFX_MYMESSAGEDLG_H__C6E16EB6_27F2_45D7_90AB_3AEE31385D25__INCLUDED_)
#define AFX_MYMESSAGEDLG_H__C6E16EB6_27F2_45D7_90AB_3AEE31385D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyButton.h"
#include "afxwin.h"
// MyMessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMessageDlg dialog

class CMyMessageDlg : public CDialog
{
// Construction
public:
	CMyMessageDlg(CWnd* pParent = NULL);   // standard constructor
	CMyMessageDlg(const CString& text, const CString& title="¸æ¾¯", int closeSeconds=5, BOOL bDefaultOnOK=TRUE, UINT uType = MB_ICONINFORMATION, CWnd* pParent = NULL);   // standard constructor

	void SetTitle(const CString& title);
	void SetMessageText(const CString& text);
	void SetCloseTime(int Seconds);

	virtual int DoModal();

// Dialog Data
	//{{AFX_DATA(CMyMessageDlg)
	enum { IDD = IDD_DIALOG_MESSAGE_DLG };
	CMyButton	m_btnOK;
	CMyButton	m_btnCancel;
	CString	m_stMsgText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nCloseTime;
	int m_nTimeLeft;
	CString m_stTitle;
	HICON m_hIcon;
	BOOL m_bIsDefaultOK;
    UINT m_uType;

	// Generated message map functions
	//{{AFX_MSG(CMyMessageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CStatic m_objIco;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMESSAGEDLG_H__C6E16EB6_27F2_45D7_90AB_3AEE31385D25__INCLUDED_)
