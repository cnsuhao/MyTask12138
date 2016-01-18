// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "SetDlg.h"
#include "PubData.h"


#include <mmsystem.h>//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
    , m_nMusicIndex(-1)
{
	//{{AFX_DATA_INIT(CSetDlg)
	m_nNameType = -1;
	m_stFileName = _T("");
	m_stBakPic = _T("");
	m_bIsRunStartUp = FALSE;
	m_stLogViewerName = _T("");
	//}}AFX_DATA_INIT
	m_bIsPlayingMusic = false;
	m_bIsBakPicChanged = false;
	m_bLastIsRunStartUp = FALSE;
	m_bIsViewerToolChanded = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
	DDX_Control(pDX, IDC_BTN_DEFAULT_LOG_VIEWER, m_btnSetDefaultViewer);
	DDX_Control(pDX, IDC_BTN_MODIFY_LOG_VIEWER, m_btnModLogViewer);
	DDX_Control(pDX, ID_BTN_CHG_BAK, m_btnChgBak);
	DDX_Control(pDX, IDC_BUTTON_STOP_M, m_btnStopM);
	DDX_Control(pDX, IDC_BUTTON_PLAY_M, m_btnPlayM);
	DDX_Control(pDX, IDC_BUTTON_EXPLORER, m_btnExplorer);
	DDX_Control(pDX, IDC_BUTTON_DEL_M, m_btnDelM);
	DDX_Control(pDX, IDC_BUTTON_ADD_M, m_btnAddM);
	DDX_Control(pDX, IDC_BTN_CO_USED, m_btnCoUsed);
	DDX_Control(pDX, IDC_BTN_CO_REDUPLICATE, m_btnCoRed);
	DDX_Control(pDX, IDC_BTN_CO_NOT_INC, m_btnCoNotInc);
	DDX_Control(pDX, IDC_BTN_CO_FREE, m_btnCoFree);
	DDX_Control(pDX, ID_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, ID_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_LIST_MUSIC, m_lstMusicName);
	DDX_Radio(pDX, IDC_RADIO_FILE_DAY, m_nNameType);
	DDX_Text(pDX, IDC_EDIT_DEFALUT_FILE, m_stFileName);
	DDX_Text(pDX, IDC_EDIT_BAK_PIC, m_stBakPic);
	DDX_Check(pDX, IDC_CHECK_START_UP, m_bIsRunStartUp);
	DDX_Text(pDX, IDC_EDIT_LOG_VIEWER, m_stLogViewerName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_M, OnButtonAddM)
	ON_BN_CLICKED(IDC_BUTTON_DEL_M, OnButtonDelM)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_M, OnButtonPlayM)
	ON_BN_CLICKED(IDC_BUTTON_EXPLORER, OnButtonExplorer)
	ON_BN_CLICKED(IDC_BUTTON_STOP_M, OnButtonStopM)
	ON_BN_CLICKED(IDC_RADIO_FILE_DAY, OnRadioFileChange)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_CO_FREE, OnBtnCoFree)
	ON_BN_CLICKED(IDC_BTN_CO_USED, OnBtnCoUsed)
	ON_BN_CLICKED(IDC_BTN_CO_REDUPLICATE, OnBtnCoReduplicate)
	ON_BN_CLICKED(IDC_BTN_CO_NOT_INC, OnBtnCoNotInc)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(ID_BTN_CHG_BAK, OnBtnChgBak)
	ON_BN_CLICKED(IDC_BTN_MODIFY_LOG_VIEWER, OnBtnModifyLogViewer)
	ON_BN_CLICKED(IDC_RADIO_FILE_MONTH, OnRadioFileChange)
	ON_BN_CLICKED(IDC_RADIO_FILE_WEEK, OnRadioFileChange)
	ON_BN_CLICKED(IDC_RADIO_FILE_USER_DEFINE, OnRadioFileChange)
	ON_BN_CLICKED(ID_BTN_SAVE, OnOK)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_LOG_VIEWER, OnBtnUseDefaultLogViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers

void CSetDlg::OnButtonAddM() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "WAV文件(*.wav)|*.wav|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"WAV文件(*.wav)|*.wav|所有文件(*.*)|*.*|",
		this);
	if (IDOK == dlg.DoModal())
	{
		m_lstMusicName.AddString(LPCTSTR(dlg.GetPathName()));
		CPubData::setMsg.AddMusic(LPCTSTR(dlg.GetPathName()));
	}
}

void CSetDlg::OnButtonDelM() 
{
	// TODO: Add your control notification handler code here
	if(m_lstMusicName.GetCurSel()<0)
	{
		return;
	}
	else
	{
		CString st;
		int index = m_lstMusicName.GetCurSel();
		st.Format("确认要删除音乐文件[%s]吗？",CPubData::GetPureName(CPubData::setMsg.GetMusicAt(index)));
		if(IDOK==::MessageBox(LPCTSTR(st), "确认删除", 10, FALSE))
		{
			m_lstMusicName.DeleteString(index);
			CPubData::setMsg.RemoveMusicAt(index);
		}
	}
}

void CSetDlg::OnButtonPlayM() 
{
	// TODO: Add your control notification handler code here
	if (m_lstMusicName.GetCurSel() < 0)
	{
		::MessageBox("请先选择要播放的音乐！","",3);
		return;
	}

	CString strMusicFile=CPubData::setMsg.GetMusicAt(m_lstMusicName.GetCurSel());//MP3文件名
	
	PlaySound(LPCTSTR(strMusicFile), NULL, SND_ALIAS|SND_ASYNC);
	m_bIsPlayingMusic = true;

	m_nMusicIndex = 0;
	GetDlgItem(IDC_BUTTON_STOP_M)-> EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PLAY_M)-> EnableWindow(FALSE);
}

void CSetDlg::OnButtonStopM() 
{
	// TODO: Add your control notification handler code here
	PlaySound(NULL, NULL, SND_ALIAS|SND_ASYNC);
	m_bIsPlayingMusic = false;
	GetDlgItem(IDC_BUTTON_STOP_M)-> EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY_M)-> EnableWindow(TRUE);
}

void CSetDlg::OnButtonExplorer() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "任务数据文件(*.dat)|*.dat|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"任务数据文件(*.dat)|*.dat|所有文件(*.*)|*.*|",
		this);
	if (IDOK == dlg.DoModal())
	{
		UpdateData();
		m_stFileName = dlg.GetPathName();
		CPubData::setMsg.SetDefaultFileName(LPCTSTR(m_stFileName));
		UpdateData(FALSE);
	}
}

void CSetDlg::OnOK()
{
	UpdateData();
	CSetMsg& setmsg = CPubData::setMsg;
	setmsg.SetDefaultFileType(m_nNameType);
	if (m_nNameType == FNT_USER_DEFINED)
	{
		setmsg.SetDefaultFileName(m_stFileName);
	}
	setmsg.SetColor(CO_NO_TASK, m_nColor[CO_NO_TASK]);
	setmsg.SetColor(CO_NOT_STARTED, m_nColor[CO_NOT_STARTED]);
	setmsg.SetColor(CO_STARTED, m_nColor[CO_STARTED]);
	setmsg.SetColor(CO_FINISHED, m_nColor[CO_FINISHED]);
	if (m_bIsPlayingMusic)
	{
		PlaySound(NULL, NULL, SND_ALIAS|SND_ASYNC);
		m_bIsPlayingMusic = false;
	}

	if (m_bIsRunStartUp != m_bLastIsRunStartUp)
	{
		CString stDir=CPubData::GetCurrentDir();
		CString stRunCmd = stDir + "\\注册启动项.exe";
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = LPCTSTR(stRunCmd); 
		ShExecInfo.lpDirectory = LPCTSTR(stDir);
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		
		if (m_bIsRunStartUp)
		{
			ShExecInfo.lpParameters = "on"; 
			ShellExecuteEx(&ShExecInfo);
		}
		else
		{
			ShExecInfo.lpParameters = "off"; 
			ShellExecuteEx(&ShExecInfo);
		}
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		setmsg.SetRunAtStartUp(m_bIsRunStartUp);
	}

	if (m_bIsViewerToolChanded)
	{
		if(m_stLogViewer.IsEmpty())
		{
			setmsg.SetLogViewer(NULL);
		}
		else
		{
			setmsg.SetLogViewer(LPCTSTR(m_stLogViewer));
		}
	}
	
	CDialog::OnOK();
}

BOOL CSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=-1;
	for(i=0; i<CPubData::setMsg.GetMusicCount(); i++)
	{
		m_lstMusicName.AddString(CPubData::setMsg.GetMusicAt(i));
	}
	m_nNameType = CPubData::setMsg.GetDefaultFileType();
	m_stFileName = CPubData::setMsg.GetDefaultFileName();
	if(m_nNameType != FNT_USER_DEFINED)
	{
		GetDlgItem(IDC_BUTTON_EXPLORER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_EXPLORER)->EnableWindow(TRUE);
	}
	for(i=0; i<MAX_COLOR; i++)
	{
		m_nColor[i] = CPubData::setMsg.GetColor(i);
	}

	if (CPubData::setMsg.IsBakPicSet())
	{
		m_stBakPic = CPubData::setMsg.GetBakPic();
	}
	else
	{
		m_stBakPic = "未设置...";
	}

	if (CPubData::setMsg.IsRunAtStartUp())
	{
		m_bLastIsRunStartUp = TRUE;
		m_bIsRunStartUp = TRUE;
	}
	else
	{
		m_bLastIsRunStartUp = FALSE;
		m_bIsRunStartUp = FALSE;
	}

	if (CPubData::setMsg.IsLogViewerSet())
	{
		m_stLogViewer = CPubData::setMsg.GetLogViewer();
		m_stLogViewerName = CPubData::GetPureName(m_stLogViewer, FALSE);
	}
	else
	{
		m_stLogViewer.Empty();
		m_stLogViewerName = "记事本";
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);			// Set small icon

	m_btnSave.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnCancel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnChgBak.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnCoFree.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnCoNotInc.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnCoRed.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnCoUsed.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnDelM.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnExplorer.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnAddM.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnPlayM.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnStopM.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnModLogViewer.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	m_btnSetDefaultViewer.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP3,IDB_BITMAP2,IDB_BITMAP4);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDlg::OnRadioFileChange() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_stFileName = CPubData::setMsg.GetDefaultFileName(m_nNameType);
	if(m_nNameType != FNT_USER_DEFINED)
	{
		GetDlgItem(IDC_BUTTON_EXPLORER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_EXPLORER)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DrawColor(dc, IDC_STATIC_FREE, m_nColor[CO_NO_TASK]);
	DrawColor(dc, IDC_STATIC_USED, m_nColor[CO_NOT_STARTED]);
	DrawColor(dc, IDC_STATIC_REDUPLICATE, m_nColor[CO_STARTED]);
	DrawColor(dc, IDC_STATIC_NOT_INCLUDE, m_nColor[CO_FINISHED]);
	// Do not call CDialog::OnPaint() for painting messages
}

void CSetDlg::DrawColor( CPaintDC& dc, int recID, COLORREF color )
{
	CRect rect;
	GetDlgItem(recID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.left += rect.Width()+10;
	rect.right = rect.left+100;
	CBrush  br(color);
	CBrush* pold = dc.SelectObject(&br);
	dc.Rectangle(&rect);
	dc.SelectObject(pold);
}

void CSetDlg::OnBtnCoFree() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (IDOK == dlg.DoModal())
	{
		m_nColor[CO_NO_TASK] = dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CSetDlg::OnBtnCoUsed() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (IDOK == dlg.DoModal())
	{
		m_nColor[CO_NOT_STARTED] = dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CSetDlg::OnBtnCoReduplicate() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (IDOK == dlg.DoModal())
	{
		m_nColor[CO_STARTED] = dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CSetDlg::OnBtnCoNotInc() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (IDOK == dlg.DoModal())
	{
		m_nColor[CO_FINISHED] = dlg.GetColor();
		Invalidate(FALSE);
	}
}

HBRUSH CSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CSetDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSetDlg::OnCancel()
{
	if (m_bIsPlayingMusic)
	{
		PlaySound(NULL, NULL, SND_ALIAS|SND_ASYNC);
		m_bIsPlayingMusic = false;
	}
	CDialog::OnCancel();
}

void CSetDlg::OnBtnChgBak() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "背景图片(*.bmp)|*.bmp|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"背景图片(*.bmp)|*.bmp|",
		this);
	if (IDOK == dlg.DoModal())
	{
		UpdateData();
		m_stBakPic = dlg.GetPathName();
		CPubData::gBr.DeleteObject();

		CBitmap bitmap;
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
			LPCTSTR(m_stBakPic), 
			IMAGE_BITMAP,
			0,
			0,
			LR_CREATEDIBSECTION|LR_LOADFROMFILE);
		bitmap.Attach(hbmp);
		//bitmap.LoadBitmap(IDB_BITMAP_BAK);
		CPubData::gBr.CreatePatternBrush(&bitmap);
		//CPubData::setMsg.SetDefaultFileName(LPCTSTR(m_stFileName));
		UpdateData(FALSE);
		Invalidate(TRUE);
		CPubData::setMsg.SetBakPic(LPCTSTR(m_stBakPic));
		m_bIsBakPicChanged = true;
	}
}

bool CSetDlg::IsResetBakPic()
{
	return m_bIsBakPicChanged;
}

void CSetDlg::OnBtnModifyLogViewer() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "可执行程序(*.exe)|*.exe|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"可执行程序(*.exe)|*.exe|脚本(*.cmd;*.bat)|*.cmd;*.bat|Perl脚本(*.pl)|*.pl|所有文件(*.*)|*.*|",
		this);
	if (IDOK == dlg.DoModal())
	{
		UpdateData();
		m_stLogViewer = dlg.GetPathName();
		m_stLogViewerName = CPubData::GetPureName(m_stLogViewer);
		m_bIsViewerToolChanded = true;
		UpdateData(FALSE);
	}
}

void CSetDlg::OnBtnUseDefaultLogViewer() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_stLogViewer.Empty();
	m_stLogViewerName = "记事本";
	m_bIsViewerToolChanded = true;
	UpdateData(FALSE);
}

bool CSetDlg::IsLogViewerChanged()
{
	return m_bIsViewerToolChanded;
}
