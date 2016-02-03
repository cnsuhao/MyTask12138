// MyDfTaskEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyDfTaskEditDlg.h"
#include "PubData.h"
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDfTaskEditDlg dialog


CMyDfTaskEditDlg::CMyDfTaskEditDlg(TaskMsg task, int type, CWnd* pParent /*=NULL*/)
	: CDialog(CMyDfTaskEditDlg::IDD, pParent),m_nType(type), m_task(task)
    , m_bIsTipSet(FALSE)
    , m_bIsOperSet(FALSE)
    , m_stCmd(_T(""))
    , m_stCertainDay(_T(""))
    , m_nDateType(RMD_TT_MAX)
{
	//{{AFX_DATA_INIT(CMyDfTaskEditDlg)
	m_stTitle = _T("");
	m_stDetails = _T("");
	m_nDays = 0;
	m_stHour = _T("");
	m_stMin = _T("");
	m_nMusicIndex = -1;
	//}}AFX_DATA_INIT
}


void CMyDfTaskEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMyDfTaskEditDlg)
    DDX_Control(pDX, IDC_COMBO_MUSIC, m_lstMusic);
    DDX_Control(pDX, IDC_COMBO_WEEK, m_lstWeek);
    DDX_Control(pDX, ID_CANCEL_EDIT_DF, m_btnCancel);
    DDX_Control(pDX, ID_OK_EDIT_DF, m_btnOK);
    DDX_Control(pDX, IDC_COMBO_MONTH, m_lstMonth);
    DDX_Text(pDX, IDC_EDIT_TITLE_DF, m_stTitle);
    DDX_Text(pDX, IDC_EDIT_DETAILS, m_stDetails);
    DDX_Text(pDX, IDC_EDIT_DAYS, m_nDays);
    DDX_CBString(pDX, IDC_COMBO_S_H, m_stHour);
    DDX_CBString(pDX, IDC_COMBO_S_M, m_stMin);
    DDX_CBIndex(pDX, IDC_COMBO_MUSIC, m_nMusicIndex);
    //}}AFX_DATA_MAP
    DDX_Check(pDX, IDC_CHECK_MUSIC_DF, m_bIsTipSet);
    DDX_Check(pDX, IDC_CHECK_OPERATOR_DF, m_bIsOperSet);
    DDX_Control(pDX, IDC_COMBO_CMD, m_ctrlCmdList);
    DDX_CBString(pDX, IDC_COMBO_CMD, m_stCmd);
    DDX_Text(pDX, IDC_EDIT_C_DAY, m_stCertainDay);
}


BEGIN_MESSAGE_MAP(CMyDfTaskEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDfTaskEditDlg)
	ON_BN_CLICKED(ID_OK_EDIT_DF, OnOkEditDf)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_CANCEL_EDIT_DF, OnCancelEditDf)
	ON_BN_CLICKED(IDC_RADIO_DAY, OnRadioDay)
	ON_BN_CLICKED(IDC_RADIO_WEEK, OnRadioWeek)
	ON_BN_CLICKED(IDC_RADIO_MONTH, OnRadioMonth)
	ON_BN_CLICKED(IDC_RADIO_USER_DEFINE, OnRadioUserDefine)
    ON_BN_CLICKED(IDC_RADIO_CERTAIN_DAY, OnRadioCertainDay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDfTaskEditDlg message handlers

void CMyDfTaskEditDlg::OnOkEditDf() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_stTitle.IsEmpty())
	{
		::MessageBox("请输入标题！", "输入错误", MB_ICONWARNING, 3);
		return;
	}
    if (m_stHour.IsEmpty() || m_stMin.IsEmpty())
    {
        ::MessageBox("请输入提醒时间！", "输入错误", MB_ICONWARNING, 3);
        GetDlgItem(IDC_EDIT_MINS)->SetFocus();
        return;
    }
    if (m_bIsOperSet && m_stCmd.IsEmpty())
    {
        ::MessageBox("请输入操作的命令！", "输入错误", MB_ICONWARNING, 3);
        GetDlgItem(IDC_EDIT_CMD)->SetFocus();
        return;
    }
    if (m_nDateType == RMD_TT_EXACT_TIME
        && !CheckDate(m_stCertainDay))
    {
        ::MessageBox("请输入正确的日期！", "输入错误", MB_ICONWARNING, 3);
        GetDlgItem(IDC_EDIT_C_DAY)->SetFocus();
        return;
    }

	GetData();
	OnOK();
}

BOOL CMyDfTaskEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString st;
	switch (m_nType)
	{
		case MDT_NEW:
			SetWindowText("新建任务");
			SetDateType(RMD_TT_PER_DAY);
			break;
		case MDT_EDIT:
			st.Format("修改任务 - [%s]", m_task.stTitle);
			SetDateType(m_task.tipNext.rt.nTypeTime);
            GetDlgItem(IDC_EDIT_TITLE_DF)->EnableWindow(FALSE);
			break;
		case MDT_SHOW:
			SetDateType(m_task.tipNext.rt.nTypeTime);
			st.Format("查看任务 - [%s]", m_task.stTitle);
			break;
		default:
			SetDateType(-1);
			SetWindowText("错误！");
			break;
	}
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

	int Count = CPubData::setMsg.GetMusicCount();
	int i;
	if (Count > 0)
	{
		while(m_lstMusic.GetCount()>0)
			m_lstMusic.DeleteString(0);
	}
	for(i=0; i<Count; i++)
	{
		m_lstMusic.AddString(CPubData::GetPureName(CPubData::setMsg.GetMusicAt(i),TRUE));
	}
	m_nMusicIndex = m_task.nMusicIndex;
	m_lstMusic.SetCurSel(m_nMusicIndex);

	for (i=0; i<31; i++)
	{
		st.Format("%02d",i+1);
		m_lstMonth.AddString(st);
	}

    POSITION pos = TConfig::GetCmds().GetStartPosition();
    CString strKey,strValue;
    while(pos)
    {
        TConfig::GetCmds().GetNextAssoc(pos,strKey,strValue);
        m_ctrlCmdList.AddString(strKey);
        ADD_DEBUG("添加默认命令[%s=[%s]]成功！", strKey,strValue);
    }

    SetData(m_task);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMyDfTaskEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CMyDfTaskEditDlg::OnCancelEditDf() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CMyDfTaskEditDlg::SetDateType( int type )
{
	static int IDs[]={IDC_RADIO_DAY, IDC_RADIO_WEEK, IDC_RADIO_MONTH, IDC_RADIO_USER_DEFINE, IDC_RADIO_CERTAIN_DAY};
	for(int i=0; i<sizeof(IDs)/sizeof(int); i++)
	{
		if(i==type)
		{
			((CButton*)GetDlgItem(IDs[i]))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDs[i]))->SetCheck(FALSE);
		}
	}
	CTime ti = CTime::GetCurrentTime();
	switch(type)
	{
		case RMD_TT_PER_DAY:
			m_nDateType = RMD_TT_PER_DAY;
            m_task.tipNext.rt.nTypeTime = RMD_TT_PER_DAY;
            m_task.tipNext.rt.nDays = 1;
			m_lstWeek.ShowWindow(SW_HIDE);
			m_lstMonth.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_DAYS)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_C_DAY)->EnableWindow(FALSE);
			break;
		case RMD_TT_PER_WEEK:
			m_nDateType = RMD_TT_PER_WEEK;
			m_lstWeek.ShowWindow(SW_SHOW);
			m_lstMonth.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_DAYS)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_C_DAY)->EnableWindow(FALSE);
			m_lstWeek.SetCurSel(ti.GetDayOfWeek()-1);
			break;
		case RMD_TT_PER_MONTH:
			m_nDateType = RMD_TT_PER_MONTH;
			m_lstWeek.ShowWindow(SW_HIDE);
			m_lstMonth.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DAYS)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_C_DAY)->EnableWindow(FALSE);
			m_lstMonth.SetCurSel(ti.GetDay()-1);
			break;
		case RMD_TT_DAYS:
			m_nDateType = RMD_TT_DAYS;
			m_lstWeek.ShowWindow(SW_HIDE);
			m_lstMonth.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_DAYS)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_C_DAY)->EnableWindow(FALSE);
			break;
        case RMD_TT_EXACT_TIME:
            m_nDateType = RMD_TT_EXACT_TIME;
            m_task.tipNext.rt.nTypeTime = RMD_TT_EXACT_TIME;
            m_task.tipNext.rt.nDays = 0;
            m_lstWeek.ShowWindow(SW_HIDE);
            m_lstMonth.ShowWindow(SW_HIDE);
            GetDlgItem(IDC_EDIT_DAYS)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_C_DAY)->EnableWindow(TRUE);
            break;
	}
}

void CMyDfTaskEditDlg::OnRadioDay() 
{
	// TODO: Add your control notification handler code here
	SetDateType(RMD_TT_PER_DAY);
}

void CMyDfTaskEditDlg::OnRadioWeek() 
{
	// TODO: Add your control notification handler code here
	SetDateType(RMD_TT_PER_WEEK);
}

void CMyDfTaskEditDlg::OnRadioMonth() 
{
	// TODO: Add your control notification handler code here
	SetDateType(RMD_TT_PER_MONTH);
}

void CMyDfTaskEditDlg::OnRadioUserDefine() 
{
	// TODO: Add your control notification handler code here
	SetDateType(RMD_TT_DAYS);
}

TaskMsg* CMyDfTaskEditDlg::GetTaskPointer()
{
	return &m_task;
}

void CMyDfTaskEditDlg::GetData()
{
	strncpy(m_task.stTitle, m_stTitle, sizeof(m_task.stTitle)-1);
    strncpy(m_task.stCmd, m_stCmd, sizeof(m_task.stCmd)-1);
	strncpy(m_task.stDetails, m_stDetails, sizeof(m_task.stDetails)-1);
	m_task.tipNext.rt.nTypeTime = m_nDateType;
    m_task.nMsgType = MT_TIMING;
    if (m_bIsTipSet || m_bIsOperSet)
    {
        if (m_bIsTipSet) m_task.nMsgType |= MT_RMD;
        if (m_bIsOperSet) m_task.nMsgType |= MT_CMD;
    }
    else
    {
        ADD_ERROR("默认任务[%s]内部错误！", m_task.stTitle);
    }
	
	switch(m_nDateType)
	{
		case RMD_TT_PER_WEEK:
			m_task.tipNext.rt.nDays = m_lstWeek.GetCurSel()+1;
			break;
		case RMD_TT_PER_MONTH:
			m_task.tipNext.rt.nDays = m_lstMonth.GetCurSel()+1;
			break;
		case RMD_TT_DAYS:
			m_task.tipNext.rt.nDays = m_nDays;
        case RMD_TT_EXACT_TIME:
            m_task.tipNext.rt.nDays = 0;
			break;
	}
	m_task.tiStartSet = 0;
	time_t tiTmp = 0;
    if (m_nDateType == RMD_TT_EXACT_TIME)
    {
        (void)CheckDate(m_stCertainDay, &tiTmp);
    }
    else
    {
        tiTmp = CTime::GetCurrentTime().GetTime();
    }
	struct tm tmStart = *(localtime(&tiTmp));
	tmStart.tm_hour = atoi(LPCTSTR(m_stHour));
	tmStart.tm_min = atoi(LPCTSTR(m_stMin));
	tmStart.tm_sec = 0;
	m_task.tiStartSet = mktime(&tmStart);
    m_task.CountNextRmdTime(true);
    ADD_NORMAL("默认任务[%s]编辑后重算执行时间[%s]", 
        m_task.stTitle, LPCTSTR(GetDateTime(m_task.tiNextRmd)));
}

void CMyDfTaskEditDlg::SetData()
{
    char sttime[32]={0};
    struct tm tmStart = *(localtime(&m_task.tiStartSet));
    strftime(sttime, 32, "%H", &tmStart);
    m_stHour = sttime;
    strftime(sttime, 32, "%M", &tmStart);
    m_stMin = sttime;

    if (m_nType == MDT_NEW)
    {
        return;
    }
	m_stTitle = m_task.stTitle;
	m_stDetails = m_task.stDetails;
    m_stCmd = m_task.stCmd;
	m_nDateType = m_task.tipNext.rt.nTypeTime;
	m_nDays = 1;

	switch(m_nDateType)
	{
		case RMD_TT_PER_WEEK:
			m_lstWeek.SetCurSel(m_task.tipNext.rt.nDays-1);
			break;
		case RMD_TT_PER_MONTH:
			m_lstMonth.SetCurSel(m_task.tipNext.rt.nDays-1);
			break;
		case RMD_TT_DAYS:
			m_nDays = m_task.tipNext.rt.nDays;
			break;
        case RMD_TT_EXACT_TIME:
            m_stCertainDay = GetDate(m_task.tiStartSet);
            break;
        case RMD_TT_PER_DAY:
        default:
			m_nDays = 1;
			break;
	}

	m_nMusicIndex = m_task.nMusicIndex;
	m_lstMusic.SetCurSel(m_nMusicIndex);

    if (CheckMsgTypeRemind(m_task.nMsgType))
    {
        m_bIsTipSet = TRUE;
    }
    else
    {
        m_bIsTipSet = FALSE;
    }

    if (CheckMsgTypeRunCmd(m_task.nMsgType))
    {
        m_bIsOperSet = TRUE;
    }
    else
    {
        m_bIsOperSet = FALSE;
    }
}

void CMyDfTaskEditDlg::SetData( const TaskMsg& tsk )
{
	m_task = tsk;
	SetData();
}

void CMyDfTaskEditDlg::OnRadioCertainDay()
{
    // TODO: 在此添加控件通知处理程序代码
    SetDateType(RMD_TT_EXACT_TIME);
}
