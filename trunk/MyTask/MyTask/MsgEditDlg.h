#if !defined(AFX_MSGEDITDLG_H__F419C46F_A0FF_4026_9C82_0B8DE0242648__INCLUDED_)
#define AFX_MSGEDITDLG_H__F419C46F_A0FF_4026_9C82_0B8DE0242648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Types.h"
#include "MyButton.h"
// MsgEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgEditDlg dialog

class CMsgEditDlg : public CDialog
{
// Construction
public:
	void GetData();
	void SetData();
	void SetData(const TaskMsg& task);
	TaskMsg* GetTaskMsgPointer();
	CMsgEditDlg(TaskMsg task, int type, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CMsgEditDlg)
	enum { IDD = IDD_DIALOG_MSG };
	CMyButton	m_btnQuitEdit;
	CMyButton	m_btnOK;
	CMyButton	m_btnCancel;
	CComboBox	m_lstMusic;
	CMonthCalCtrl	m_ctlCalendar;
	int		m_nMusicIndex;
	CString	m_stCmd;
	CString	m_stDate;
	CString	m_stDetails;
	CString	m_stTitle;
	CString	m_stEndHour;
	CString	m_stEndMin;
	CString	m_stStartHour;
	CString	m_stStartMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bDateEdit;
	TaskMsg m_myTask;
	CTime m_tiTime;
	int m_nOPType;

protected:
	void SetShow();
	bool CheckData();
	LPCTSTR GetPureFileName( LPCTSTR pst );

	// Generated message map functions
	//{{AFX_MSG(CMsgEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetfocusEditDate();
	afx_msg void OnSelchangeMonthcalendarMine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectMonthcalendarMine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnQuitEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSelchangeComboSH();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGEDITDLG_H__F419C46F_A0FF_4026_9C82_0B8DE0242648__INCLUDED_)
