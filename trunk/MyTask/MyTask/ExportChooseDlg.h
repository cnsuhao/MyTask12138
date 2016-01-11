#if !defined(AFX_EXPORTCHOOSEDLG_H__3CC5C31F_326A_47EC_BA18_1FE357B19C42__INCLUDED_)
#define AFX_EXPORTCHOOSEDLG_H__3CC5C31F_326A_47EC_BA18_1FE357B19C42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyButton.h"
// ExportChooseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportChooseDlg dialog

class CExportChooseDlg : public CDialog
{
// Construction
public:
	int GetExportType();
	int GetExportIndex();
	void OnCancel();
	CExportChooseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportChooseDlg)
	enum { IDD = IDD_DIALOG_EXPORT };
	CMyButton	m_btnCur;
	CMyButton	m_btnAllByDay;
	CMyButton	m_btnAll;
	CMyButton	m_btnCancel;
	int		m_nIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportChooseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nExportType;

	// Generated message map functions
	//{{AFX_MSG(CExportChooseDlg)
	afx_msg void OnBtnCur();
	afx_msg void OnBtnAll();
	afx_msg void OnBtnAllByDay();
	afx_msg void OnCancelExport();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRadioExcel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTCHOOSEDLG_H__3CC5C31F_326A_47EC_BA18_1FE357B19C42__INCLUDED_)
