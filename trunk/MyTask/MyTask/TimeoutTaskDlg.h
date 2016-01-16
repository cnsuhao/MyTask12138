#pragma once
#include "afxwin.h"
#include "mybutton.h"


// CTimeoutTaskDlg 对话框

class CTimeoutTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeoutTaskDlg)

public:
	CTimeoutTaskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeoutTaskDlg();

	virtual void OnFinalRelease();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIMEOUT_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedOkKnown();

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
    CListBox m_listTasks;
    CMyButton m_btnAllKnown;
};
