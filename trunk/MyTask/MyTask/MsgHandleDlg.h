#if !defined(AFX_MSGHANDLEDLG_H__5146860A_05D6_4778_8506_2A9775FBFBC6__INCLUDED_)
#define AFX_MSGHANDLEDLG_H__5146860A_05D6_4778_8506_2A9775FBFBC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Types.h"
#include "MyButton.h"
// MsgHandleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgHandleDlg dialog

class CMsgHandleDlg : public CDialog
{
// Construction
public:
	void OnCancel();
	int GetRemindType(int* pmins = NULL);
	CMsgHandleDlg(const TaskMsg& task, CWnd* pParent = NULL);   // standard constructor
	bool IsSubTask();

// Dialog Data
	//{{AFX_DATA(CMsgHandleDlg)
	enum { IDD = IDD_DIALOG_HANDLE_MSG };
	CMyButton	m_btnOKAndEdit;
	CMyButton	m_btnOK;
	CComboBox	m_ctlRmdType;
	int		m_nMinToRemind;
	CString	m_stDetails;
	CString	m_stETime;
	CString	m_stSTime;
	CString	m_stTitle;
	CString	m_stPercent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgHandleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nRmdType;
	bool m_bSubTask;
	const TaskMsg& m_myTask;
	CRect m_rtBtn;
	CRect m_rtBtn2;

protected:
	void StopMusic();

	// Generated message map functions
	//{{AFX_MSG(CMsgHandleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnSelchangeComboRemind();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMsgHandleOkAndEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CString m_stEndTimeTitle;
public:
    CString m_stStartTimeTitle;
public:
    CString m_stPrcntTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGHANDLEDLG_H__5146860A_05D6_4778_8506_2A9775FBFBC6__INCLUDED_)
