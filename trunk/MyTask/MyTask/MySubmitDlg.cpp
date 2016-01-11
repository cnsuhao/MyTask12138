// MySubmitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PubData.h"
#include "MyTask.h"
#include "MySubmitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySubmitDlg dialog


CMySubmitDlg::CMySubmitDlg(int tlastper, const char* name, CWnd* pParent /*=NULL*/)
	: CDialog(CMySubmitDlg::IDD, pParent),m_stTitle(name), m_nLastPer(tlastper)
{
	//{{AFX_DATA_INIT(CMySubmitDlg)
	m_stPer = _T("");
	//}}AFX_DATA_INIT
}


void CMySubmitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySubmitDlg)
	DDX_Control(pDX, ID_SUB_OK, m_btnOK);
	DDX_Control(pDX, ID_BTN_SUB_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_PER, m_ctlPerChose);
	DDX_CBString(pDX, IDC_COMBO_PER, m_stPer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySubmitDlg, CDialog)
	//{{AFX_MSG_MAP(CMySubmitDlg)
	ON_CBN_DROPDOWN(IDC_COMBO_PER, OnDropdownComboPer)
	ON_CBN_SELENDCANCEL(IDC_COMBO_PER, OnSelendcancelComboPer)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_SUB_CANCEL, OnCancel)
	ON_BN_CLICKED(ID_SUB_OK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySubmitDlg message handlers

void CMySubmitDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_stPer.IsEmpty())
	{
		::MessageBox("请选择或输入当前任务进度","数据错误",3);
		return;
	}
	int nPer = atoi(LPCTSTR(m_stPer));
	if (nPer < 0 || nPer>100)
	{
		::MessageBox("进度值错误，只能是[0,100]","数据错误",3);
		return;
	}
	CDialog::OnOK();
}

int CMySubmitDlg::GetPercent()
{
	return atoi(LPCTSTR(m_stPer));
}

BOOL CMySubmitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString st;
	st.Format("修改进度 - %s", LPCTSTR(m_stTitle));
	SetWindowText(LPCTSTR(st));
	m_stPer.Format("%d",m_nLastPer);
	UpdateData(FALSE);

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);

	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMySubmitDlg::OnDropdownComboPer() 
{
	// TODO: Add your control notification handler code here
	m_ctlPerChose.SetCurSel(m_nLastPer/10 + 1);
}

void CMySubmitDlg::OnSelendcancelComboPer() 
{
	// TODO: Add your control notification handler code here
// 	UpdateData();
// 	m_stPer.Format("%d",m_nLastPer);
// 	UpdateData(FALSE);
}

HBRUSH CMySubmitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	{
		pDC->SetBkMode(TRANSPARENT);
		return CPubData::gBr;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CMySubmitDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
