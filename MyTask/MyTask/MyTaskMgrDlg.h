#if !defined(AFX_MYTASKMGRDLG_H__EB12C8CD_020A_4C62_BA3D_9AC264606519__INCLUDED_)
#define AFX_MYTASKMGRDLG_H__EB12C8CD_020A_4C62_BA3D_9AC264606519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyButton.h"
#include "Types.h"
#include "afxwin.h"
// MyTaskMgrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTaskMgrDlg dialog

class CMyTaskMgrDlg : public CDialog
{
// Construction
public:
	CMyTaskMgrDlg(CWnd* pParent = NULL);   // standard constructor
	void AddDfTask(const TaskMsg& tsk, int index);

// Dialog Data
	//{{AFX_DATA(CMyTaskMgrDlg)
	enum { IDD = IDD_DIALOG_TASKS_MGR };
	CListCtrl	m_ctlListAll;
	CMyButton	m_btnExit;
	CMyButton	m_btnMod;
	CMyButton	m_btnDel;
	CMyButton	m_btnAdd;
    CMyButton   m_btnExport;
    CMyButton   m_btnImport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTaskMgrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetDataToCtrl(const TaskMsg& tsk, int index, bool IsAdd);

	// Generated message map functions
	//{{AFX_MSG(CMyTaskMgrDlg)
	afx_msg void OnEditDfAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditDfModify();
	afx_msg void OnEditDfDel();
	afx_msg void OnOkEditDf();
    afx_msg void OnBnClickedEditDfExport();
    afx_msg void OnBnClickedEditDfImport();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTASKMGRDLG_H__EB12C8CD_020A_4C62_BA3D_9AC264606519__INCLUDED_)
