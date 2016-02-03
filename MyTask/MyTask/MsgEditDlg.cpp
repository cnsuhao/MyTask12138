// MsgEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MsgEditDlg.h"
#include "PubData.h"
#include "Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgEditDlg dialog

CMsgEditDlg::CMsgEditDlg(TaskMsg task, int type, CWnd* pParent /*=NULL*/)
: CDialog(CMsgEditDlg::IDD, pParent), m_myTask(task),m_nOPType(type)
{
	//{{AFX_DATA_INIT(CMsgEditDlg)
	//}}AFX_DATA_INIT
	m_nMusicIndex = 0;
	m_bDateEdit = false;
}

void CMsgEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgEditDlg)
	DDX_Control(pDX, IDC_BTN_QUIT_EDIT, m_btnQuitEdit);
	DDX_Control(pDX, ID_EDIT_OK, m_btnOK);
	DDX_Control(pDX, ID_EDIT_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_MUSIC, m_lstMusic);
	DDX_Control(pDX, IDC_MONTHCALENDAR_MINE, m_ctlCalendar);
	DDX_CBIndex(pDX, IDC_COMBO_MUSIC, m_nMusicIndex);
	DDX_Text(pDX, IDC_EDIT_CMD, m_stCmd);
	DDV_MaxChars(pDX, m_stCmd, 127);
	DDX_Text(pDX, IDC_EDIT_DATE, m_stDate);
	DDX_Text(pDX, IDC_EDIT_DETAILS, m_stDetails);
	DDV_MaxChars(pDX, m_stDetails, 127);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_stTitle);
	DDV_MaxChars(pDX, m_stTitle, 31);
	DDX_CBString(pDX, IDC_COMBO_E_H, m_stEndHour);
	DDV_MaxChars(pDX, m_stEndHour, 2);
	DDX_CBString(pDX, IDC_COMBO_E_M, m_stEndMin);
	DDV_MaxChars(pDX, m_stEndMin, 2);
	DDX_CBString(pDX, IDC_COMBO_S_H, m_stStartHour);
	DDV_MaxChars(pDX, m_stStartHour, 2);
	DDX_CBString(pDX, IDC_COMBO_S_M, m_stStartMin);
	DDV_MaxChars(pDX, m_stStartMin, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgEditDlg)
	ON_WM_KILLFOCUS()
	ON_EN_SETFOCUS(IDC_EDIT_DATE, OnSetfocusEditDate)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR_MINE, OnSelchangeMonthcalendarMine)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR_MINE, OnSelectMonthcalendarMine)
	ON_BN_CLICKED(IDC_BTN_QUIT_EDIT, OnBtnQuitEdit)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_S_H, OnSelchangeComboSH)
	ON_BN_CLICKED(ID_EDIT_OK, OnOK)
	ON_BN_CLICKED(ID_EDIT_CANCEL, OnCancel)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgEditDlg message handlers

TaskMsg* CMsgEditDlg::GetTaskMsgPointer()
{
	GetData();
	return &m_myTask;
}

void CMsgEditDlg::SetData()
{
	char sttime[32];
	struct tm tmStart = *(localtime(&m_myTask.tiStartSet));
	struct tm tmEnd = *(localtime(&m_myTask.tipNext.tiEndSet));
	m_nMusicIndex = 0;
	m_stCmd = m_myTask.stCmd;
	strftime(sttime, 32, "%H", &tmEnd);
	m_stEndHour = sttime;
	strftime(sttime, 32, "%M", &tmEnd);
	m_stEndMin = sttime;
	strftime(sttime, 32, "%H", &tmStart);
	m_stStartHour = sttime;
	strftime(sttime, 32, "%M", &tmStart);
	m_stStartMin = sttime;
	m_stCmd = m_myTask.stCmd;
	strftime(sttime, 32, "%Y-%m-%d", &tmStart);
	m_stDate = sttime;
	m_stDetails = m_myTask.stDetails;
	m_stTitle = m_myTask.stTitle;
	m_tiTime = mktime(&tmStart);
	m_nMusicIndex = m_myTask.nMusicIndex;
	m_lstMusic.SetCurSel(m_nMusicIndex);
	UpdateData(FALSE);
}

void CMsgEditDlg::SetData( const TaskMsg& task )
{
	m_myTask = task;
	SetData();
}

BOOL CMsgEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LPCTSTR pst;
	int i=-1;
	if (CPubData::setMsg.GetMusicCount() != 0)
	{
		m_lstMusic.DeleteString(0);
	}
	for (i=0; i<CPubData::setMsg.GetMusicCount(); i++)
	{
		pst = CPubData::setMsg.GetMusicAt(i);
		pst = GetPureFileName(pst);
		m_lstMusic.AddString(pst);
	}
	// TODO: Add extra initialization here
	SetData();
	SetShow();
	UpdateData(FALSE);

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);

	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnQuitEdit.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgEditDlg::OnKillFocus(CWnd* pNewWnd) 
{
	//CDialog::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	
}

void CMsgEditDlg::GetData()
{
	m_myTask.nMusicIndex = m_nMusicIndex;
	strncpy(m_myTask.stCmd, m_stCmd, 128);
	m_myTask.stCmd[127] = '\0';
	strncpy(m_myTask.stTitle, m_stTitle, 32);
	m_myTask.stTitle[31] = '\0';
	strncpy(m_myTask.stDetails, m_stDetails, 128);
	m_myTask.stDetails[127] = '\0';

	time_t tiTmp=m_tiTime.GetTime();
	struct tm tmStart = *(localtime(&tiTmp));
	struct tm tmEnd = *(localtime(&tiTmp));
	tmStart.tm_hour = atoi(LPCTSTR(m_stStartHour));
	tmStart.tm_min = atoi(LPCTSTR(m_stStartMin));
	tmStart.tm_sec = 0;
	m_myTask.tiStartSet = mktime(&tmStart);
	tmEnd.tm_hour = atoi(LPCTSTR(m_stEndHour));
	tmEnd.tm_min = atoi(LPCTSTR(m_stEndMin));
	tmEnd.tm_sec = 0;
	m_myTask.tipNext.tiEndSet = mktime(&tmEnd);
}

void CMsgEditDlg::OnSetfocusEditDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bDateEdit)
	{
		m_ctlCalendar.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_QUIT_EDIT)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_6)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_COMBO_S_H)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_S_M)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_E_H)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_E_M)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_EDIT_DETAILS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TITLE)->ShowWindow(SW_HIDE);
	}
}

void CMsgEditDlg::OnSelchangeMonthcalendarMine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CMsgEditDlg::OnOK()
{
	if(!UpdateData())
	{
		return;
	}
	if(!CheckData())
	{
		return;
	}
	CDialog::OnOK();
}

void CMsgEditDlg::OnSelectMonthcalendarMine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SYSTEMTIME sysTime;
	m_ctlCalendar.GetCurSel(&sysTime);
	sysTime.wHour = sysTime.wMinute = sysTime.wSecond =  sysTime.wMilliseconds = 0;
	m_tiTime = sysTime;

	m_ctlCalendar.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_QUIT_EDIT)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_6)->ShowWindow(SW_SHOW);
	
	GetDlgItem(IDC_COMBO_S_H)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_S_M)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_E_H)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_E_M)->ShowWindow(SW_SHOW);
	
	GetDlgItem(IDC_EDIT_DETAILS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_TITLE)->ShowWindow(SW_SHOW);
	
	m_stDate = m_tiTime.Format("%Y-%m-%d");
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CMsgEditDlg::OnBtnQuitEdit() 
{
	// TODO: Add your control notification handler code here
	m_ctlCalendar.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_QUIT_EDIT)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_6)->ShowWindow(SW_SHOW);
	
	GetDlgItem(IDC_COMBO_S_H)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_S_M)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_E_H)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_E_M)->ShowWindow(SW_SHOW);
	
	GetDlgItem(IDC_EDIT_DETAILS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_TITLE)->ShowWindow(SW_SHOW);
	
	UpdateData(FALSE);
}

void CMsgEditDlg::SetShow()
{
	CString st;
	if (MDT_NEW == m_nOPType)
	{
		// st.Format("新增提醒 - %s", m_myTask.stTitle);
		SetWindowText("新增提醒");
		m_bDateEdit = true;
	}
	else if (MDT_EDIT == m_nOPType)
	{
		st.Format("修改提醒 - %s", m_myTask.stTitle);
		SetWindowText(LPCTSTR(st));
        if (TConfig::IsTaskTitleModifiable())
        {
            ((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetReadOnly(FALSE);
        }
        else
        {
            ((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetReadOnly(TRUE);
        }
		m_bDateEdit = true;
	}
	else if (MDT_SHOW == m_nOPType)
	{
		st.Format("查看提醒 - %s", m_myTask.stTitle);
		SetWindowText(LPCTSTR(st));
		((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetReadOnly(TRUE);
		GetDlgItem(IDC_COMBO_S_H)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_S_M)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_E_H)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_E_M)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MUSIC)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_DETAILS))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_CMD))->SetReadOnly(TRUE);
		m_bDateEdit = false;
	}
}

bool CMsgEditDlg::CheckData()
{
	int nStart = atoi(LPCTSTR(m_stStartHour))*60+atoi(LPCTSTR(m_stStartMin));
	int nEnd = atoi(LPCTSTR(m_stEndHour))*60+atoi(LPCTSTR(m_stEndMin));
	if (nStart > nEnd)
	{
		::MessageBox("结束时间不能比起始时间早", "输入错误", MB_ICONINFORMATION, 3);
		return false;
	}

	if (m_stTitle.GetLength() == 0)
	{
		::MessageBox("标题不能为空！", "输入错误", MB_ICONWARNING, 3);
		return false;
	}

	return true;
}

HBRUSH CMsgEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CMsgEditDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CMsgEditDlg::OnSelchangeComboSH() 
{
	// TODO: Add your control notification handler code here
}

LPCTSTR CMsgEditDlg::GetPureFileName( LPCTSTR pst )
{
	int i = strlen(pst) - 1;
	while(pst[i] != '\\')
	{
		i--;
	}
	return pst + i + 1;
}

int CMsgEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
