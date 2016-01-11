#if !defined(AFX_SETDLG_H__703BEB24_4B31_472D_9CAB_9A91ECBFE13E__INCLUDED_)
#define AFX_SETDLG_H__703BEB24_4B31_472D_9CAB_9A91ECBFE13E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Types.h"
#include "MyButton.h"
// SetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

class CSetDlg : public CDialog
{
// Construction
public:
	bool IsLogViewerChanged();
	bool IsResetBakPic();
	void DrawColor(CPaintDC& dc, int recID, COLORREF color);
	void OnOK();
	CSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_DIALOG_SET };
	CMyButton	m_btnSetDefaultViewer;
	CMyButton	m_btnModLogViewer;
	CMyButton	m_btnChgBak;
	CMyButton	m_btnStopM;
	CMyButton	m_btnPlayM;
	CMyButton	m_btnExplorer;
	CMyButton	m_btnDelM;
	CMyButton	m_btnAddM;
	CMyButton	m_btnCoUsed;
	CMyButton	m_btnCoRed;
	CMyButton	m_btnCoNotInc;
	CMyButton	m_btnCoFree;
	CMyButton	m_btnSave;
	CMyButton	m_btnCancel;
	CListBox	m_lstMusicName;
	int		m_nNameType;
	CString	m_stFileName;
	CString	m_stBakPic;
	BOOL	m_bIsRunStartUp;
	CString	m_stLogViewerName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bIsViewerToolChanded;
	bool m_bIsBakPicChanged;
	void OnCancel();
	bool m_bIsPlayingMusic;
	int m_nMusicIndex;
	COLORREF m_nColor[MAX_COLOR];
	BOOL m_bLastIsRunStartUp;
	CString m_stLogViewer;

	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	afx_msg void OnButtonAddM();
	afx_msg void OnButtonDelM();
	afx_msg void OnButtonPlayM();
	afx_msg void OnButtonExplorer();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStopM();
	afx_msg void OnRadioFileChange();
	afx_msg void OnPaint();
	afx_msg void OnBtnCoFree();
	afx_msg void OnBtnCoUsed();
	afx_msg void OnBtnCoReduplicate();
	afx_msg void OnBtnCoNotInc();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnChgBak();
	afx_msg void OnBtnModifyLogViewer();
	afx_msg void OnBtnUseDefaultLogViewer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDLG_H__703BEB24_4B31_472D_9CAB_9A91ECBFE13E__INCLUDED_)
