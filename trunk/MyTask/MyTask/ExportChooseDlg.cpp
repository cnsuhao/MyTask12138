// ExportChooseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "ExportChooseDlg.h"
#include "Types.h"
#include "PubData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportChooseDlg dialog


CExportChooseDlg::CExportChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportChooseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportChooseDlg)
	m_nIndex = 0;
	//}}AFX_DATA_INIT
	m_nExportType = ET_EXPORT_NONE;
}


void CExportChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportChooseDlg)
	DDX_Control(pDX, IDC_BTN_CUR, m_btnCur);
	DDX_Control(pDX, IDC_BTN_ALL_BY_DAY, m_btnAllByDay);
	DDX_Control(pDX, IDC_BTN_ALL, m_btnAll);
	DDX_Control(pDX, ID_CANCEL_EXPORT, m_btnCancel);
	DDX_Radio(pDX, IDC_RADIO_EXCEL, m_nIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportChooseDlg, CDialog)
	//{{AFX_MSG_MAP(CExportChooseDlg)
	ON_BN_CLICKED(IDC_BTN_CUR, OnBtnCur)
	ON_BN_CLICKED(IDC_BTN_ALL, OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_ALL_BY_DAY, OnBtnAllByDay)
	ON_BN_CLICKED(ID_CANCEL_EXPORT, OnCancelExport)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_EXCEL, OnRadioExcel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportChooseDlg message handlers

void CExportChooseDlg::OnBtnCur() 
{
	// TODO: Add your control notification handler code here
	m_nExportType = ET_EXPORT_CUR;
	OnOK();
}

void CExportChooseDlg::OnBtnAll() 
{
	// TODO: Add your control notification handler code here
	m_nExportType = ET_EXPORT_ALL;
	OnOK();
}

void CExportChooseDlg::OnBtnAllByDay() 
{
	// TODO: Add your control notification handler code here
	m_nExportType = ET_EXPORT_BY_DAY;
	OnOK();
}

void CExportChooseDlg::OnCancelExport() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CExportChooseDlg::OnCancel()
{
	m_nExportType = ET_EXPORT_NONE;
	CDialog::OnCancel();
}

int CExportChooseDlg::GetExportType()
{
	return m_nExportType;
}

BOOL CExportChooseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnAll.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnAllByDay.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnCur.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CExportChooseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CExportChooseDlg::OnRadioExcel() 
{
	// TODO: Add your control notification handler code here
	
}

int CExportChooseDlg::GetExportIndex()
{
	return m_nIndex;
}
