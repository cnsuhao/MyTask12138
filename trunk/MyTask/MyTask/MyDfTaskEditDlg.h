#if !defined(AFX_MYDFTASKEDITDLG_H__2F562E1E_15F1_40CD_B845_AF5F933FE3EA__INCLUDED_)
#define AFX_MYDFTASKEDITDLG_H__2F562E1E_15F1_40CD_B845_AF5F933FE3EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyButton.h"
#include "Types.h"
#include "afxwin.h"
#include "afxdtctl.h"
// MyDfTaskEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDfTaskEditDlg dialog

class CMyDfTaskEditDlg : public CDialog
{
// Construction
public:
	CMyDfTaskEditDlg(TaskMsg task, int type, CWnd* pParent = NULL);   // standard constructor
	TaskMsg* GetTaskPointer();

// Dialog Data
	//{{AFX_DATA(CMyDfTaskEditDlg)
	enum { IDD = IDD_DIALOG_DF_TASK_EDIT };
	CComboBox	m_lstMusic;
	CComboBox	m_lstWeek;
	CMyButton	m_btnCancel;
	CMyButton	m_btnOK;
	CComboBox	m_lstMonth;
	CString	m_stTitle;
	CString	m_stDetails;
	int		m_nDays;
	CString	m_stHour;
	CString	m_stMin;
	int		m_nMusicIndex;
    BOOL m_bIsTipSet;
    BOOL m_bIsOperSet;
    CComboBox m_ctrlCmdList;
    CString m_stCmd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDfTaskEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	TaskMsg m_task;
	int m_nType;
	int m_nDateType;

protected:
	void GetData();
	void SetData();
	void SetData(const TaskMsg& tsk);
	void SetDateType( int type );

	// Generated message map functions
	//{{AFX_MSG(CMyDfTaskEditDlg)
	afx_msg void OnOkEditDf();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCancelEditDf();
	afx_msg void OnRadioDay();
	afx_msg void OnRadioWeek();
	afx_msg void OnRadioMonth();
	afx_msg void OnRadioUserDefine();
    afx_msg void OnRadioCertainDay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CString m_stCertainDay;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDFTASKEDITDLG_H__2F562E1E_15F1_40CD_B845_AF5F933FE3EA__INCLUDED_)
