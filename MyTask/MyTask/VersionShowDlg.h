#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
#include "MyButton.h"


// CVersionShowDlg 对话框

class CVersionShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CVersionShowDlg)

public:
	CVersionShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionShowDlg();
    void SetVersionInfo(const char* st[], int size_);

// 对话框数据
	enum { IDD = IDD_DIALOG_VERSION_SHOW };

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    typedef std::vector<std::string> VecString;
    typedef std::vector<VecString*>  VecVersions;
    VecString vecTitle;
    VecVersions vecVers;

	DECLARE_MESSAGE_MAP()
    void DoSelectVersion( int index );
public:
    CComboBox m_ctrlVerList;
public:
    CString m_stVerList;
public:
    CListBox m_ctrlDetailsList;
public:
    afx_msg void OnCbnSelchangeComboVerList();
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
    CMyButton m_btnExit;
};
