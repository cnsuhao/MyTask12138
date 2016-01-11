// MsgHandleDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MsgHandleDlg.h"
#include "PubData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgHandleDlg dialog
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

CMsgHandleDlg::CMsgHandleDlg(const TaskMsg& task, CWnd* pParent /*=NULL*/)
: CDialog(CMsgHandleDlg::IDD, pParent), m_myTask(task)
, m_stEndTimeTitle(_T(""))
, m_stStartTimeTitle(_T(""))
, m_stPrcntTitle(_T(""))
{
	//{{AFX_DATA_INIT(CMsgHandleDlg)
	m_nMinToRemind = 15;
	m_stDetails = _T("");
	m_stETime = _T("");
	m_stSTime = _T("");
	m_stTitle = _T("");
	m_stPercent = _T("");
	//}}AFX_DATA_INIT
	m_bSubTask = false;
}


void CMsgHandleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgHandleDlg)
    DDX_Control(pDX, ID_MSG_HANDLE_OK_AND_EDIT, m_btnOKAndEdit);
    DDX_Control(pDX, ID_MSG_HANDLE_OK, m_btnOK);
    DDX_Control(pDX, IDC_COMBO_REMIND, m_ctlRmdType);
    DDX_Text(pDX, IDC_EDIT_MINS, m_nMinToRemind);
    DDX_Text(pDX, IDC_STATIC_DETAILS, m_stDetails);
    DDX_Text(pDX, IDC_STATIC_ETIME, m_stETime);
    DDX_Text(pDX, IDC_STATIC_STIME, m_stSTime);
    DDX_Text(pDX, IDC_STATIC_TITLE, m_stTitle);
    DDX_Text(pDX, IDC_STATIC_PERCENT, m_stPercent);
    DDX_Text(pDX, IDC_STATIC_END_TIME, m_stEndTimeTitle);
    DDX_Text(pDX, IDC_STATIC_START_TIME, m_stStartTimeTitle);
    DDX_Text(pDX, IDC_STATIC_PERCENT_TITLE, m_stPrcntTitle);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgHandleDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgHandleDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_REMIND, OnSelchangeComboRemind)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_MSG_HANDLE_OK, OnOK)
	ON_BN_CLICKED(ID_MSG_HANDLE_OK_AND_EDIT, OnMsgHandleOkAndEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMsgHandleDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

    //Sleep(4*1000);
    //MessageBox("Before CMsgHandleDlg::OnInitDialog");
	CTime ti;
	m_stTitle = m_myTask.stTitle;
	ti = m_myTask.tiStartSet;
	m_stSTime = ti.Format("%Y-%m-%d %H:%M:%S");
	m_stDetails = m_myTask.stDetails;

	m_ctlRmdType.SetCurSel(0);
	GetDlgItem(ID_MSG_HANDLE_OK)->GetWindowRect(&m_rtBtn);
	ScreenToClient(&m_rtBtn);
	GetDlgItem(ID_MSG_HANDLE_OK_AND_EDIT)->GetWindowRect(&m_rtBtn2);
	ScreenToClient(&m_rtBtn2);
	CRect rect(m_rtBtn);
	rect.left -= 200;
	rect.right -= 200;
	GetDlgItem(ID_MSG_HANDLE_OK)->MoveWindow(&rect);
	CRect rect2 = m_rtBtn2;
	rect2.left -= 200;
	rect2.right -= 200;
	GetDlgItem(ID_MSG_HANDLE_OK_AND_EDIT)->MoveWindow(&rect2);
	GetDlgItem(IDC_STATIC_X1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MINS)->ShowWindow(SW_HIDE);

    if (m_myTask.nMsgType == MT_COMMON)
    {
        m_stPrcntTitle = "处理百分比:";
        m_stPercent.Format("%2d%%",m_myTask.nPercent);
        m_stStartTimeTitle = "开始时间:";
        m_stEndTimeTitle = "结束时间:";
        ti = m_myTask.tipNext.tiEndSet;
        m_stETime = ti.Format("%Y-%m-%d %H:%M:%S");
    }
    else if (m_myTask.nMsgType != MT_UNDEFINED)
    {
        m_stPrcntTitle = "处理命令:";
        if (CheckMsgTypeRunCmd(m_myTask.nMsgType) 
            && CheckMsgTypeRemind(m_myTask.nMsgType))
        {
            m_stPercent = "提醒并执行[";
            m_stPercent += m_myTask.stCmd;
            m_stPercent += "]";
        }
        else if (CheckMsgTypeRunCmd(m_myTask.nMsgType) 
            && !CheckMsgTypeRemind(m_myTask.nMsgType))
        {
            m_stPercent = "执行[";
            m_stPercent += m_myTask.stCmd;
            m_stPercent += "]";
        }
        else if (!CheckMsgTypeRunCmd(m_myTask.nMsgType) 
            && CheckMsgTypeRemind(m_myTask.nMsgType))
        {
            m_stPercent = "只提醒";
        }
        else
        {
            m_stPercent = "ERROR";
        }
        m_stStartTimeTitle = "开始时间:";
        m_stEndTimeTitle = "下次提醒时间:";
        ti = m_myTask.tiNextRmd;
        m_stETime = ti.Format("%Y-%m-%d %H:%M:%S");
        m_ctlRmdType.EnableWindow(FALSE);
    }

	CString st = "提醒 - " + m_stTitle;
	SetWindowText(LPCTSTR(st));
	UpdateData(FALSE);

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);

	PlaySound(LPCTSTR(CPubData::setMsg.GetMusicAt(m_myTask.nMusicIndex)), NULL, SND_ALIAS|SND_ASYNC);
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnOKAndEdit.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
    //Sleep(4*1000);
    //MessageBox("After OnInitDialog");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMsgHandleDlg message handlers

void CMsgHandleDlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nRmdType = m_ctlRmdType.GetCurSel();
	StopMusic();
	CDialog::OnOK();
}

void CMsgHandleDlg::OnSelchangeComboRemind() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_ctlRmdType.GetCurSel() == RMD_USER_DEFINE)
	{
		CRect rect2 = m_rtBtn2;
		GetDlgItem(ID_MSG_HANDLE_OK_AND_EDIT)->MoveWindow(&rect2);
		CRect rect(m_rtBtn);
		GetDlgItem(ID_MSG_HANDLE_OK)->MoveWindow(&rect);

		GetDlgItem(IDC_STATIC_X1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MINS)->ShowWindow(SW_SHOW);
	}
	else
	{
		CRect rect2 = m_rtBtn2;
		rect2.left -= 200;
		rect2.right -= 200;
		GetDlgItem(ID_MSG_HANDLE_OK_AND_EDIT)->MoveWindow(&rect2);
		CRect rect(m_rtBtn);
		rect.left -= 200;
		rect.right -= 200;
		GetDlgItem(ID_MSG_HANDLE_OK)->MoveWindow(&rect);
		
		GetDlgItem(IDC_STATIC_X1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MINS)->ShowWindow(SW_HIDE);
	}
	m_nRmdType = m_ctlRmdType.GetCurSel();
}

int CMsgHandleDlg::GetRemindType( int* pmins /*= NULL*/ )
{
	if(pmins == NULL)
	{
		return -1;
	}

	int type = m_nRmdType;
	if (type != RMD_USER_DEFINE)
	{
		switch(type)
		{
		case RMD_5_MIN:
			*pmins = 5;
			break;
		case RMD_10_MIN:
			*pmins = 10;
			break;
		case RMD_30_MIN:
			*pmins = 30;
			break;
		case RMD_1_HOUR:
			*pmins = 60;
			break;
		case RMD_NOT_REMIND:
			*pmins = 0;
			break;
		}
	}
	else
	{
		*pmins = m_nMinToRemind;
	}
	return type;
}

HBRUSH CMsgHandleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CMsgHandleDlg::StopMusic()
{
	PlaySound(NULL, NULL, SND_ALIAS|SND_ASYNC);
}

void CMsgHandleDlg::OnCancel()
{
	StopMusic();
	CDialog::OnCancel();
}

void CMsgHandleDlg::OnMsgHandleOkAndEdit() 
{
	// TODO: Add your control notification handler code here
	m_bSubTask = true;
	OnOK();
}

bool CMsgHandleDlg::IsSubTask()
{
	return m_bSubTask;
}
