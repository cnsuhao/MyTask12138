// MyMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyMessageDlg.h"
#include "PubData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyMessageDlg dialog


CMyMessageDlg::CMyMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyMessageDlg::IDD, pParent)
    , m_nCloseTime(0)
    , m_nTimeLeft(0)
{
	//{{AFX_DATA_INIT(CMyMessageDlg)
	m_stMsgText = _T("");
	//}}AFX_DATA_INIT
	m_bIsDefaultOK = TRUE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMyMessageDlg::CMyMessageDlg( const CString& text, const CString& title/*="告警"*/, int closeSeconds/*=5*/,BOOL bDefaultOnOK/*=TRUE*/, CWnd* pParent /*= NULL*/ )
    : CDialog(CMyMessageDlg::IDD, pParent)
    , m_nCloseTime(closeSeconds)
    , m_stTitle(title)
    , m_stMsgText(text)
    , m_bIsDefaultOK(bDefaultOnOK)
    , m_nTimeLeft(closeSeconds)
{
	m_stTitle += " - " + CPubData::GetPureModuleName();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyMessageDlg)
	DDX_Control(pDX, ID_MESDLG_OK, m_btnOK);
	DDX_Control(pDX, ID_MESDLG_CANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_MSGTEXT, m_stMsgText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMyMessageDlg)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_MESDLG_OK, OnOK)
	ON_BN_CLICKED(ID_MESDLG_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMessageDlg message handlers

BOOL CMyMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetWindowText(LPCTSTR(m_stTitle));
	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);			// Set big icon
	SetTimer(0, 1000, NULL);
	m_nTimeLeft = m_nCloseTime;
	CString st;
	if(m_bIsDefaultOK)
	{
		st.Format("确定(%u)",m_nTimeLeft);
		m_btnOK.SetWindowText(LPCTSTR(st));
	}
	else
	{
		st.Format("取消(%u)",m_nTimeLeft);
		m_btnCancel.SetWindowText(LPCTSTR(st));
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyMessageDlg::SetTitle( const CString& title )
{
	m_stTitle = title;
	SetWindowText(title);
}

void CMyMessageDlg::SetMessageText( const CString& text )
{
	m_stMsgText = text;
	UpdateData(FALSE);
}

void CMyMessageDlg::SetCloseTime( int Seconds )
{
	m_nCloseTime = Seconds;
}

void CMyMessageDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		m_nTimeLeft --;
		CString st;
		if(m_bIsDefaultOK)
		{
			st.Format("确定(%u)",m_nTimeLeft);
			m_btnOK.SetWindowText(LPCTSTR(st));
		}
		else
		{
			st.Format("取消(%u)",m_nTimeLeft);
			m_btnCancel.SetWindowText(LPCTSTR(st));
		}
		
		if (m_nTimeLeft==0)
		{
			if(m_bIsDefaultOK)
			{
				OnOK();
			}
			else
			{
				OnCancel();
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMyMessageDlg::OnOK()
{
	KillTimer(0);
	CDialog::OnOK();
}

void CMyMessageDlg::OnCancel()
{
	KillTimer(0);
	CDialog::OnCancel();
}

int CMyMessageDlg::DoModal()
{

	return CDialog::DoModal();
}

HBRUSH CMyMessageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
