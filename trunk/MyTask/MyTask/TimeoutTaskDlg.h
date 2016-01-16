#pragma once
#include "afxwin.h"
#include "mybutton.h"


// CTimeoutTaskDlg �Ի���

class CTimeoutTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeoutTaskDlg)

public:
	CTimeoutTaskDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeoutTaskDlg();

	virtual void OnFinalRelease();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIMEOUT_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
