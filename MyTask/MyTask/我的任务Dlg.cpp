// 我的任务Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "excel.h"
#include "MyTask.h"
#include "我的任务Dlg.h"
#include "MsgEditDlg.h"
#include "SetDlg.h"
#include "PubData.h"
#include "MySubmitDlg.h"
#include "MsgHandleDlg.h"
#include "MyFileDialog.h"
#include <winuser.h>
#include "MyLog.h"
#include "comdef.h"
#include <comutil.h>
#include "ExportChooseDlg.h"

#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
#include "MyTaskMgrDlg.h"
#include "Types.h"
#include "tinyxml/tinyxml.h"
#include "VersionShowDlg.h"
#include "TimeoutTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	void OnCancel();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CMyButton	m_btnShowDetails;
	CMyButton	m_btnOK;
	CString	m_stVer;
	CString	m_stEmail;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDetails();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_stEmail = _T("sunlight12345@qq.com");
	//}}AFX_DATA_INIT
	char st[MAX_PATH];
	GetModuleFileName(NULL, st, MAX_PATH);
	CString stName = CPubData::GetPureName(st,FALSE);
	m_stVer.Format("%s v%d.%d", LPCTSTR(stName), VER_HIGH, VER_LOW);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, ID_DETAILS, m_btnShowDetails);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_STATIC_VER, m_stVer);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_stEmail);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_DETAILS, OnDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnOK.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnShowDetails.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	//if (pWnd == this)
	{
		pDC->SetBkMode(TRANSPARENT);
		return CPubData::gBr;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
void CAboutDlg::OnDetails() 
{
	// TODO: Add your control notification handler code here
    CVersionShowDlg dlg;
    dlg.DoModal();
	//CDialog::OnOK();
}

void CAboutDlg::OnCancel()
{
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
HICON IconID[2];

const char* TipTitles[TT_COUNT]=
{
	"开始工作啦！",
	"你完成了吗？",
	"你还记得吗？",
    "定时提醒！",
    "循环提醒！"
};

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
    , m_iSortIndex(-1)
    , m_bIsAscending(true)
    , m_bIsOnStart(true)
{
	//{{AFX_DATA_INIT(CMyDlg)
	m_stStatus = _T("就绪");
	m_nSelectDate = SI_ALL;
	m_stFileType = _T("");
	m_stCurTime = _T("");
	m_stShowDate = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nLastDateSel = m_nSelectDate;
	m_nTipIndex = -1;
	m_nTipType = -1;
// 	m_pEditDlg = NULL;
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Control(pDX, IDC_COMBO_SELECT, m_ctlSelectDate);
	DDX_Control(pDX, IDC_LIST_TASK_SHOW2, m_ctlListTmp);
	DDX_Control(pDX, IDC_LIST_TASK_SHOW, m_ctlList);
	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_BTN_HIDE, m_btnHide);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_stStatus);
	DDX_CBIndex(pDX, IDC_COMBO_SELECT, m_nSelectDate);
	DDX_Text(pDX, IDC_STATIC_FILE, m_stFileType);
	DDX_Text(pDX, IDC_CUR_TIME, m_stCurTime);
	DDX_Text(pDX, IDC_STATIC_TIME, m_stShowDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_BTN_TOOL_ADD, OnButtonToolAdd)
	ON_COMMAND(ID_BTN_TOOL_DEL, OnButtonToolDel)
	ON_COMMAND(ID_MENU_DEL, OnMenuDel)
	ON_COMMAND(ID_BTN_TOOL_EDIT, OnButtonToolEdit)
	ON_COMMAND(ID_BTN_TOOL_OPEN, OnButtonToolOpen)
	ON_COMMAND(ID_BTN_TOOL_OPEN_DEFAULT, OnButtonToolOpenDefault)
	ON_COMMAND(ID_BTN_TOOL_SAVE, OnButtonToolSave)
	ON_COMMAND(ID_BTN_TOOL_SAVE_DEFAULT, OnButtonToolSaveDefault)
	ON_COMMAND(ID_BTN_TOOL_SAVE_AS, OnButtonToolSaveAs)
	ON_COMMAND(ID_BTN_TOOL_SET, OnButtonToolSet)
	ON_COMMAND(ID_BTN_TOOL_SUB, OnButtonToolSub)
	ON_COMMAND(ID_BTN_TOOL_SUB_FINISH, OnButtonToolFinish)
	ON_COMMAND(ID_BTN_TOOL_EXIT, OnButtonToolExit)
	ON_COMMAND(ID_BTN_TOOL_EXPORT, OnButtonToolExport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TASK_SHOW, OnClickListTaskShow)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_TASK_SHOW, OnColumnclickListTaskShow)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TASK_SHOW, OnRclickListTaskShow)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT, OnSelchangeComboSelect)
	ON_MESSAGE(MYWM_NOTIFYICON,OnNotifyTray)
	ON_MESSAGE(MY_MSG_SHOW,OnMsgShow)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(IDM_MAIN_SUBMIT, OnUpdateMainSubmit)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDM_MAIN_VIEW_LOG, OnMainViewLog)
	ON_COMMAND(IDM_MAIN_EXPORT, OnExportCur)
	ON_COMMAND(IDM_MAIN_SET_VIEW_TOOL, OnMainSetViewTool)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK_SHOW, OnDblclkListTaskShow)
	ON_BN_CLICKED(IDC_STATIC_MY_ICON, OnStaticMyIcon)
	ON_BN_CLICKED(IDOK, OnButtonToolExit)
	ON_COMMAND(IDM_MAIN_OPEN, OnButtonToolOpen)
	ON_COMMAND(IDM_MAIN_OPEN_DEFAULT, OnButtonToolOpenDefault)
	ON_COMMAND(IDM_MAIN_SAVE, OnButtonToolSave)
	ON_COMMAND(IDM_MAIN_SAVE_AS, OnButtonToolSaveAs)
	ON_COMMAND(IDM_MAIN_SAVE_DEFAULT, OnButtonToolOpenDefault)
	ON_COMMAND(IDM_MAIN_EXIT, OnButtonToolExit)
	ON_COMMAND(IDM_MAIN_ADD, OnButtonToolAdd)
	ON_COMMAND(IDM_MAIN_DEL, OnButtonToolDel)
	ON_COMMAND(IDM_MAIN_EDIT, OnButtonToolEdit)
	ON_COMMAND(IDM_MAIN_SUBMIT, OnButtonToolSub)
	ON_COMMAND(IDM_MAIN_FINISH, OnButtonToolFinish)
	ON_COMMAND(IDM_MAIN_COPY, OnMenuCopyTask)
	ON_COMMAND(IDM_MAIN_CLEAR_COPY, OnMenuClearCopy)
	ON_COMMAND(IDM_MAIN_PASTE, OnMenuPasteTask)
	ON_COMMAND(IDM_MAIN_SET, OnButtonToolSet)
	ON_COMMAND(IDM_MAIN_OPEN_DEFAULT, OnButtonToolOpenDefault)
	ON_COMMAND(IDM_MENU_EXIT, OnButtonToolExit)
	ON_COMMAND(IDM_MENU_SHOW, OnMyShowWindow)
	ON_COMMAND(IDM_MENU_SET, OnButtonToolSet)
	ON_COMMAND(IDM_MAIN_DF_TASKS, OnMainDfTasks)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED,OnTaskBarCreated)
	ON_MESSAGE(NIN_BALLOONTIMEOUT, OnTrayIconTipOff)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_32839, &CMyDlg::OnAbout)
    ON_COMMAND(IDM_MAIN_INI_EDIT, &CMyDlg::OnMenuMainIniEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	UpdateData(TRUE);
	SetWindowText(MAIN_DLG_NAME);
	CTime ti=CTime::GetCurrentTime();
	m_stCurTime = ti.Format("%H:%M:%S");
	
	CString st = ti.Format("%Y年%m月%d日");
	m_stShowDate.Format("今天是%s", LPCTSTR(st));
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

    TConfig::SetLogLevel(LVL_INFO);
	WriteOperatorLog("=====================================================");
	st.Format("我的任务 v%d.%d 启动...", VER_HIGH, VER_LOW);
	WriteOperatorLog(LPCTSTR(st));

    // 读取配置文件
    TConfig::ReadAllConfig();
    m_stFileType = CPubData::GetFileTypeName();
    if (CPubData::setMsg.IsLogViewerSet())
    {
        m_stOpenLogCmd = CPubData::setMsg.GetLogViewer();
    }

	// 装载主菜单
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	// 创建工具栏
	if (! m_ctlToolBar.CreateEx( this,TBSTYLE_FLAT ,  WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS,
		CRect(10,5,0,0)) ||    ! m_ctlToolBar.LoadToolBar(IDR_TOOLBAR_MAIN) )
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}
	m_ctlToolBar.ModifyStyle(0, TBSTYLE_TRANSPARENT);

	{
        int i=0;
        UINT id, style;
        int image;
        CString strTipText;
        for(i=0; i<m_ctlToolBar.GetCount(); i++)
        {
            m_ctlToolBar.GetButtonInfo(i, id, style, image);
            strTipText.LoadString(id);   
            strTipText.Replace('\n','\0');
            m_ctlToolBar.SetButtonText(i, LPCTSTR(strTipText));
        }
        //m_ctlToolBar.EnableDocking(CBRS_ALIGN_TOP);
        WORD cx = LOWORD(m_ctlToolBar.GetToolBarCtrl().GetButtonSize());
        WORD cy = HIWORD(m_ctlToolBar.GetToolBarCtrl().GetButtonSize());
        m_ctlToolBar.GetToolBarCtrl().SetButtonSize(CSize(cx, cy));

	}
//	m_ctlToolBar.AdjustLayout();
	m_ctlToolBar.ShowWindow(SW_SHOW);
	//m_ctlToolBar.GetToolBarCtrl().SetButtonWidth(43, 70);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	// 初始化表格
	DWORD dwStyle = m_ctlList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_OWNERDRAWFIXED;
	m_ctlList.SetExtendedStyle(dwStyle); //设置扩展风格
	m_ctlList.SetBkColor(CLR_NONE);

	m_ctlList.InsertColumn( 0, CPubData::gstListTitles[0], LVCFMT_LEFT, 60 );//插入列
	m_ctlList.InsertColumn( 1, CPubData::gstListTitles[1], LVCFMT_LEFT, 200 );//插入列
	m_ctlList.InsertColumn( 2, CPubData::gstListTitles[2], LVCFMT_CENTER, 154 );
	m_ctlList.InsertColumn( 3, CPubData::gstListTitles[3], LVCFMT_CENTER, 154 );
	m_ctlList.InsertColumn( 4, CPubData::gstListTitles[4], LVCFMT_RIGHT, 80 );
	m_ctlList.InsertColumn( 5, CPubData::gstListTitles[5], LVCFMT_CENTER, 154 );
	m_ctlList.InsertColumn( 6, CPubData::gstListTitles[6], LVCFMT_LEFT, 262 );

	m_ctlListTmp.SetExtendedStyle(dwStyle); //设置扩展风格
	
	m_ctlListTmp.InsertColumn( 0, CPubData::gstListTitles[0], LVCFMT_LEFT, 60 );//插入列
	m_ctlListTmp.InsertColumn( 1, CPubData::gstListTitles[1], LVCFMT_LEFT, 200 );//插入列
	m_ctlListTmp.InsertColumn( 2, CPubData::gstListTitles[2], LVCFMT_CENTER, 154 );
	m_ctlListTmp.InsertColumn( 3, CPubData::gstListTitles[3], LVCFMT_CENTER, 154 );
	m_ctlListTmp.InsertColumn( 4, CPubData::gstListTitles[4], LVCFMT_RIGHT, 60 );
	m_ctlListTmp.InsertColumn( 5, CPubData::gstListTitles[5], LVCFMT_CENTER, 154 );
	m_ctlListTmp.InsertColumn( 6, CPubData::gstListTitles[6], LVCFMT_LEFT, 262 );
	m_ctlListTmp.ShowWindow(SW_HIDE);

	//MessageBox(GetDate(time(NULL)));

	// 加载定时提醒数据
	CString stTmp = CPubData::GetDataFileName(FILE_NAME_DF_TASK);
	if (! DoOpenDfTasks (stTmp))
	{
		m_stStatus.Format("打开文件[%s]失败！", LPCTSTR(stTmp));
	}
	else
	{
		m_stStatus.Format("打开文件[%s]成功！", LPCTSTR(stTmp));
		//m_stCurFileName = CPubData::setMsg.GetDefaultFileName();
	}
	UpdateData(FALSE);

	// 加载普通任务数据
	if (strlen(CPubData::setMsg.GetDefaultFileName())==0)
	{
		m_stCurFileName = "";
		InitSomeTasks();
	}
	else
	{
		if (!DoOpen(CPubData::setMsg.GetDefaultFileName()))
		{
			m_stStatus.Format("打开文件[%s]失败！",CPubData::setMsg.GetDefaultFileName());
			//::MessageBox(m_stStatus,"失败");
		}
		else
		{
			m_stStatus.Format("打开文件[%s]成功！",CPubData::setMsg.GetDefaultFileName());
			m_stCurFileName = CPubData::setMsg.GetDefaultFileName();
		}
	}
	RebuildDate();
	m_ctlList.SetTextBkColor(CLR_NONE);
	//RedrawListCtrl();

	// 初始化托盘图标
	m_nIconIndex = 0;
	m_tnid.cbSize=sizeof(NOTIFYICONDATA);
	m_tnid.hWnd=this->m_hWnd;					//自定
	m_tnid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;	//自定
	m_tnid.uCallbackMessage=MYWM_NOTIFYICON;	//用户自定的回调信息
	CString stTip;
	stTip.Format("我的任务 v%d.%d\r\n单击显示主界面\r\n右击显示菜单",
		VER_HIGH,VER_LOW);
	strcpy(m_tnid.szTip,LPCTSTR(stTip));						//鼠标停留时提示信息
	m_tnid.uID=IDR_MAINFRAME;						//图标资源标号
	IconID[0]= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	IconID[1]= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME2));
	m_tnid.hIcon = IconID[m_nIconIndex];
	::Shell_NotifyIcon(NIM_ADD,&m_tnid);		//添加托盘图标


// 	CTime tiTmp = CTime::GetCurrentTime();
// 	CString stTmp;
// 	stTmp = GetDateTime(GetDateInt(tiTmp.GetTime())) + "\r\n" + GetDateTime(tiTmp.GetTime());
// 	MessageBox(stTmp);
// 	stTmp = GetDateTime(GetDateInt(tiTmp.GetTime() - 9*3600))+ "\r\n" + GetDateTime(tiTmp.GetTime() - 9*3600);
// 	MessageBox(stTmp);

	// 初始化时间轴
	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&rect);
	m_objRect = rect;
	m_ctlList.GetClientRect(&rect);
	m_ctlList.ClientToScreen(&rect);
	ScreenToClient(&rect);
	m_objRect.left += rect.left;
	m_objRect.right = m_objRect.left + 1080;
	m_objRect.top = m_objRect.bottom - 60;
	m_objRect.bottom = m_objRect.bottom - 40;
	if (IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1)
		RebuildTimeShaft();
// 	CString st;
// 	st.Format("%d",m_objRect.Width());
// 	MessageBox(st);

	// 初始化提示信息
	SetTips();

	// 初始化计时器
	SetTimer(TIMER_REMIND, 1000, NULL);
	SetTimer(TIMER_SAVE, 1000*5, NULL);
	SetChange(false);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString stPic = CPubData::setMsg.GetBakPic();
	if (CPubData::setMsg.IsBakPicSet() && !stPic.IsEmpty())
	{
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
			LPCTSTR(stPic), 
			IMAGE_BITMAP,
			0,
			0,
			LR_CREATEDIBSECTION|LR_LOADFROMFILE);
		m_myBitmap.Attach(hbmp);
	}
	else
	{
		m_myBitmap.LoadBitmap(IDB_BITMAP_BAK);
	}
	CPubData::gBr.CreatePatternBrush(&m_myBitmap);

	//m_ctlList.SetBkImage(HBITMAP(m_myBitmap));

	MENUINFO menu;
	menu.cbSize = sizeof(MENUINFO);
	menu.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND; //子菜单也变色
	HBRUSH br = CPubData::gBr.operator HBRUSH(); // CreateSolidBrush(RGB(173,201,243));
	menu.hbrBack = br;
	CMenu* pMenu = GetMenu();//但一定要在资源里定义一个菜单并调用,不能用工程自动生成的一个菜单
	SetMenuInfo(pMenu->m_hMenu,&menu);
	DrawMenuBar();

	m_btnExit.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnHide.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

    UpdateData(FALSE);

    OnStart(); // 过期提醒批量处理

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect = m_objRect;       

		CBrush br, * pold = NULL;
		CString st;
		int nLast = 5;
		dc.SetBkMode(TRANSPARENT);
		for(int i=1;i<m_arrTimeNode.GetSize(); i++)
		{
			if(m_arrTimeNode[i-1].bIsOdd)
			{
				if(m_arrTimeNode[i-1].nPos*2 % 60 == 0)
				{
					st.Format("%02d:%02d",m_arrTimeNode[i-1].nPos*2 / 60 - 1, 59);
				}
				else
				{
					st.Format("%02d:%02d",m_arrTimeNode[i-1].nPos*2 / 60, m_arrTimeNode[i-1].nPos*2 % 60 - 1);
				}
				
				rect.left = m_objRect.left + m_arrTimeNode[i-1].nPos/2*3;
				rect.right= m_objRect.left + m_arrTimeNode[i].nPos/2*3;
			}
			else
			{
				st.Format("%02d:%02d",m_arrTimeNode[i-1].nPos*2 / 60, m_arrTimeNode[i-1].nPos*2 % 60);
				rect.left = m_objRect.left + m_arrTimeNode[i-1].nPos/2*3;
				rect.right= m_objRect.left + m_arrTimeNode[i].nPos/2*3;
			}
			
			br.CreateSolidBrush(CPubData::setMsg.GetColor(m_nColorPt[m_arrTimeNode[i-1].nPos]));
			pold = dc.SelectObject(&br);
			dc.Rectangle(&rect);
			if (i>1 && m_arrTimeNode[i-1].nPos == m_arrTimeNode[i-2].nPos)
			{

			}
			else if (i>1 && m_arrTimeNode[i-1].nPos - m_arrTimeNode[i-2].nPos < 30)
			{
				// 距离过载，输出到下一行
				if (nLast == 5)
					nLast = 15;
				else
					nLast = 5;
				dc.TextOut(rect.left-18, rect.bottom+nLast, st);
			}
			else
			{
				dc.TextOut(rect.left-18, rect.bottom+5, st);
				nLast = 5;
			}
			dc.SelectObject(pold);
			br.DeleteObject();
		}
		if(m_arrTimeNode.GetSize()>0)
		{
			// 绘制整点标志
			CPen pen, *poldpen;
			pen.CreatePen(2, 1, COLOR_TIME);
			dc.SetTextColor(COLOR_TIME);
			poldpen = dc.SelectObject(&pen);
			int Span = 1080/24;
			dc.TextOut(m_objRect.left+Span*0-18, m_objRect.top-16, "00:00");
			for(int i=1; i<24; i++)
			{
				st.Format("%02d:00",i);
				dc.MoveTo(m_objRect.left+Span*i, m_objRect.top);
				dc.LineTo(m_objRect.left+Span*i, m_objRect.bottom);
				dc.TextOut(m_objRect.left+Span*i-18, m_objRect.top-16, st);
			}
			dc.TextOut(m_objRect.left+Span*24-18, m_objRect.top-16, "24:00");
			dc.SelectObject(poldpen);
		}
		//dc.Rectangle(&m_objRect);
		//dc.Rectangle(&m_pRect[1]);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnButtonToolAdd() 
{
	// TODO: Add your command handler code here
	TaskMsg task;
	CMsgEditDlg dlg(task, MDT_NEW, this);
	if(dlg.DoModal()==IDOK)
	{
		task = *(dlg.GetTaskMsgPointer());
		m_arrTasks.Add(task);
		RedrawListCtrl(m_ctlList.GetItemCount());
		m_nSelectDate = m_ctlSelectDate.GetCurSel();
		RebuildDate();
		m_ctlSelectDate.SetCurSel(m_nSelectDate);
		if (IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1)
			RebuildTimeShaft();
		else
			ClearTimeShaft();
		SetChange(true);
		m_stStatus.Format("添加任务[%s]成功！",task.stTitle);
		UpdateData(FALSE);
	}
}

void CMyDlg::OnButtonToolDel()
{
	int i;
	bool bFound = false;
	int count = 0;
	CString stTitles="删除任务";
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		// m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || 
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		m_stStatus = "★没有选中要删除的数据";
		UpdateData(FALSE);
		return;
	}
	if (::MessageBox("确认删除？","确认", MB_ICONINFORMATION, 10, FALSE) == IDOK)
	{
		while (1)
		{
			bFound = false;
			for(i=0; i<m_ctlList.GetItemCount(); i++)
			{
				// m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || 
				if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
				{
					m_ctlList.DeleteItem(i);
					stTitles += "[";
					stTitles += m_arrTasks[m_arrIndexMap.GetAt(i)].stTitle;
					stTitles += "]";
					m_arrTasks.RemoveAt(m_arrIndexMap.GetAt(i));
					bFound = true;
					count ++;
					break;
				}
			}
			
			if (!bFound)
				break;
		}
		m_stStatus = stTitles + "成功！";
		m_nSelectDate = m_ctlSelectDate.GetCurSel();
		RebuildDate();
		m_ctlSelectDate.SetCurSel(m_nSelectDate);
		if ((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
			RebuildTimeShaft();
		else
			ClearTimeShaft();
		SetChange(true);
		UpdateData(FALSE);
	}
}

void CMyDlg::OnMenuDel()
{
	int i;
	bool bFound = false;
	int item = -1;
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			bFound = true;
			item = i;
			break;
		}
	}
	if (!bFound)
	{
		m_stStatus = "★没有选中要删除的数据";
		UpdateData(FALSE);
		return;
	}
	m_ctlList.DeleteItem(i);
	CString stTmp = m_arrTasks[m_arrIndexMap.GetAt(i)].stTitle;
	m_arrTasks.RemoveAt(m_arrIndexMap.GetAt(i));
	m_stStatus.Format("删除数据[%s]成功！", LPCTSTR(stTmp));
	m_nSelectDate = m_ctlSelectDate.GetCurSel();
	RebuildDate();
	m_ctlSelectDate.SetCurSel(m_nSelectDate);
	if ((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
			RebuildTimeShaft();
	else
			ClearTimeShaft();
	SetChange(true);
	UpdateData(FALSE);
}

void CMyDlg::OnButtonToolEdit()
{
		int i;
		bool bFound = false;
		int count = 0;
		int first = -1;
		for(i=0; i<m_ctlList.GetItemCount(); i++)
		{
			if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
			{
				if(!bFound)
					first = i;
				bFound = true;
				count ++;
			}
		}

		if (!bFound)
		{
			m_stStatus = "★没有选中要编辑的数据";
			UpdateData(FALSE);
			return;
		}

		CMsgEditDlg dlg(m_arrTasks[m_arrIndexMap.GetAt(first)], MDT_EDIT, this);
		if (dlg.DoModal() == IDOK)
		{
			TaskMsg task = *(dlg.GetTaskMsgPointer());
			m_arrTasks[m_arrIndexMap.GetAt(first)] = task;
			RedrawListCtrl(first);
			SetChange(true);
			m_stStatus.Format("编辑任务[%s]成功！", task.stTitle);
			UpdateData(FALSE);
			if((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount()==DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
				RebuildTimeShaft();
			else
				ClearTimeShaft();
		}

		return;
}

void CMyDlg::OnButtonToolSub()
{
	int i;
	bool bFound = false;
	int count = 0;
	int first = -1;
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			if(!bFound)
				first = i;
			bFound = true;
			count ++;
		}
	}
		
	if (!bFound)
	{
		m_stStatus = "★没有选中要提交进度的数据";
		UpdateData(FALSE);
		return;
	}

	i = first;
	first = m_arrIndexMap[i];
	if(m_arrTasks[first].nPercent <100)
	{
		CMySubmitDlg dlg(m_arrTasks[first].nPercent, m_arrTasks[first].stTitle, this);
		if (dlg.DoModal() == IDOK)
		{
			int x = dlg.GetPercent();
			if (x>m_arrTasks[first].nPercent)
			{
				m_arrTasks[first].nPercent = x;
				if (x>=100)
				{
					m_arrTasks[first].tiEnd = time(NULL);
					m_arrTasks[first].nIsTipEnded = 1;
				}
				if((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount()==DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
					RebuildTimeShaft();
				else
						ClearTimeShaft();
				RedrawListCtrl(i);
				m_stStatus.Format("提交任务[%s]进度到[%d%%]成功！", m_arrTasks[first].stTitle, x);
				SetChange(true);
			}
			else
			{
				m_stStatus.Format("提交任务[%s]进度错误：提交后进度要比提交前大！", m_arrTasks[first].stTitle);
			}
		}
	}
	else
	{
		m_stStatus.Format("提交任务[%s]进度错误：提交后进度要比提交前大！", m_arrTasks[first].stTitle);
	}
	
	UpdateData(FALSE);
	return;
}

void CMyDlg::OnButtonToolFinish()
{
	int i;
	bool bFound = false;
	int count = 0;
	int first = -1;
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			if(!bFound)
				first = i;
			bFound = true;
			count ++;
		}
	}
		
	if (!bFound)
	{
		m_stStatus = "★没有选中要提交进度的数据";
		UpdateData(FALSE);
		return;
	}

	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			first = m_arrIndexMap[i];
			if(m_arrTasks[first].nPercent <100)
			{
				m_arrTasks[first].nPercent = 100;
				m_arrTasks[first].tiEnd = time(NULL);
				m_arrTasks[first].nIsTipEnded = 1;
				RedrawListCtrl(i);
				if((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount()==DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
					RebuildTimeShaft();
				else
					ClearTimeShaft();
				m_stStatus.Format("完成任务[%s]成功！", m_arrTasks[first].stTitle);
				SetChange(true);
			}
			else
			{
				m_stStatus.Format("完成任务[%s]错误：该任务已经完成！", m_arrTasks[first].stTitle);
			}
		}
	}
	
	UpdateData(FALSE);
	return;
}

void CMyDlg::OnButtonToolOpen()
{
	FILE* fp = NULL;
	CMyFileDialog dlg(TRUE, "任务数据文件(*.dat)|*.dat|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"任务数据文件(*.dat)|*.dat|所有文件(*.*)|*.*|",
		this);
	if (IDOK == dlg.DoModal())
	{
		UpdateData();
		if (!DoOpen(dlg.GetPathName()))
		{
			m_stStatus.Format("打开文件[%s]失败！",LPCTSTR(dlg.GetPathName()));
			::MessageBox(m_stStatus,"失败", MB_ICONERROR, 3);
		}
		else
		{
			m_stStatus.Format("打开文件[%s]成功！",LPCTSTR(dlg.GetPathName()));
			m_stCurFileName = dlg.GetPathName();
		}
		UpdateData(FALSE);
	}
	if (IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1)
		RebuildTimeShaft();
	else
		ClearTimeShaft();
}

void CMyDlg::OnButtonToolOpenDefault()
{
	UpdateData();
	if (!DoOpen(CPubData::setMsg.GetDefaultFileName()))
	{
		m_stStatus.Format("打开文件[%s]失败！",CPubData::setMsg.GetDefaultFileName());
		::MessageBox(m_stStatus,"失败", MB_ICONERROR);
	}
	else
	{
		m_stStatus.Format("打开文件[%s]成功！",CPubData::setMsg.GetDefaultFileName());
		m_stCurFileName = CPubData::setMsg.GetDefaultFileName();
	}
	if (IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1)
		RebuildTimeShaft();
	else
		ClearTimeShaft();
	UpdateData(FALSE);
}

void CMyDlg::OnButtonToolSave()
{
	UpdateData(TRUE);
	CString stTmp=m_stCurFileName;
	if (stTmp.IsEmpty())
	{
		stTmp = CPubData::setMsg.GetDefaultFileName();
	}
	if (stTmp.IsEmpty())
	{
		OnButtonToolSaveAs();
	}
	else if (!DoSave(stTmp))
	{
		m_stStatus.Format("保存文件[%s]失败！", LPCTSTR(stTmp));
		::MessageBox(m_stStatus,"保存失败", MB_ICONERROR);
	}
	else
	{
		m_stStatus.Format("保存文件[%s]成功！", LPCTSTR(stTmp));
		SetChange(false);
	}
	UpdateData(FALSE);
}

void CMyDlg::OnButtonToolSaveDefault()
{
	UpdateData(TRUE);
	CString stTmp=CPubData::setMsg.GetDefaultFileName();
	if (stTmp.IsEmpty())
	{
		m_stStatus.Format("保存为默认文件失败：未指定默认文件名！", LPCTSTR(stTmp));
		::MessageBox(m_stStatus + "\r\n请先选择设置，设置默认的文件名","保存失败", MB_ICONWARNING);
	}
	else if (!DoSave(stTmp))
	{
		m_stStatus.Format("保存文件[%s]失败！", LPCTSTR(stTmp));
		::MessageBox(m_stStatus,"保存失败", MB_ICONERROR);
	}
	else
	{
		m_stStatus.Format("保存文件[%s]成功！", LPCTSTR(stTmp));
		SetChange(false);
	}
	UpdateData(FALSE);
}

void CMyDlg::OnButtonToolSaveAs()
{
	CMyFileDialog dlg(FALSE, "任务数据文件(*.dat)|*.dat|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"任务数据文件(*.dat)|*.dat|所有文件(*.*)|*.*|",
		this);
	
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		if (!DoSave(dlg.GetPathName()))
		{
			m_stStatus.Format("保存文件[%s]失败！", LPCTSTR(dlg.GetPathName()));
			::MessageBox(m_stStatus,"保存失败", MB_ICONERROR);
		}
		else
		{
			m_stStatus.Format("保存文件[%s]成功！", LPCTSTR(dlg.GetPathName()));
			SetChange(false);
		}
		UpdateData(FALSE);
	}
}


void CMyDlg::OnButtonToolSet()
{
	CSetDlg dlg;
	CString lastBakPic = CPubData::setMsg.GetBakPic();
	if (dlg.DoModal() == IDOK)
	{
		UpdateData();
		CPubData::SaveData();
		m_stFileType = CPubData::GetFileTypeName();
		UpdateData(FALSE);
		if (dlg.IsLogViewerChanged())
		{
			if (CPubData::setMsg.IsLogViewerSet())
			{
				m_stOpenLogCmd = CPubData::setMsg.GetLogViewer();
			}
			else
			{
				m_stOpenLogCmd = "";
			}
		}
		if (dlg.IsResetBakPic())
		{
			Invalidate(TRUE);
		}
	}
	else // 以取消按钮退出设置，则还原原来的背景颜色
	{
		if(dlg.IsResetBakPic())
		{
			CPubData::setMsg.SetBakPic(LPCTSTR(lastBakPic));
			CPubData::gBr.DeleteObject();
			CPubData::gBr.CreatePatternBrush(&m_myBitmap);
		}
	}
	
}

void CMyDlg::OnButtonToolExit()
{
	OnRealExit();
}

void CMyDlg::OnCancel()
{
	//CDialog::OnCancel();
	ShowWindow(SW_HIDE);
}

void CMyDlg::OnOK()
{
	OnRealExit();
}

bool CMyDlg::RedrawListCtrl()
{
	int count= m_arrTasks.GetSize();
	m_ctlList.DeleteAllItems();

	m_ctlList.SetRedraw(FALSE);
	
	CString stDate;
	int i = 0;
    int viewIndex=0;

	m_arrIndexMap.RemoveAll();

    if(m_ctlSelectDate.GetCurSel()>=0)
    {
        m_nSelectDate = m_ctlSelectDate.GetCurSel();
    }
    m_ctlSelectDate.GetLBText(m_nSelectDate, stDate);

    for (i = 0; i < count; i++)
    {
        if (CheckShow(m_arrTasks[i], stDate))
        {
            m_stStatus.Format("★正在加载数据[%d%%]...",i*100/count);
            UpdateData(FALSE);
            m_arrIndexMap.Add(i);
            SetDataToListCtrl(viewIndex, TRUE);
            ++viewIndex;
        }
    }

	//if(stDate == "所有")
	//{
	//	for (i = 0; i < count; i++)
	//	{
	//		m_stStatus.Format("★正在加载数据[%d%%]...",i*100/count);
	//		UpdateData(FALSE);
	//		m_arrIndexMap.Add(i);
	//		SetDataToListCtrl(i, TRUE);
	//	}
	//}
	//else if(stDate == "今日")
	//{
	//	CString stTmp = GetDate(time(NULL));
	//	int viewIndex=0;
	//	for (i = 0; i < count; i++)
	//	{
	//		if(stTmp == GetDate(m_arrTasks[i].tiStartSet))
	//		{
	//			m_stStatus.Format("★正在加载数据[%d%%]...",i*100/count);
	//			UpdateData(FALSE);
	//			m_arrIndexMap.Add(i);
	//			SetDataToListCtrl(viewIndex, TRUE);
	//			viewIndex++;
	//		}
	//	}
	//}
	//else // 日期
	//{
	//	int viewIndex=0;
	//	for (i = 0; i < count; i++)
	//	{
	//		if(stDate == GetDate(m_arrTasks[i].tiStartSet))
	//		{
	//			m_stStatus.Format("★正在加载数据[%d%%]...",i*100/count);
	//			UpdateData(FALSE);
	//			m_arrIndexMap.Add(i);
	//			SetDataToListCtrl(viewIndex, TRUE);
	//			viewIndex++;
	//		}
	//	}
	//}

	m_ctlList.SetRedraw(TRUE);
	m_ctlList.Invalidate();
	m_ctlList.UpdateWindow();

	m_stStatus.Format("★加载[%s]数据完成！",LPCTSTR(stDate));
	UpdateData(FALSE);

	return true;
}

bool CMyDlg::RedrawListCtrl( int item )
{
	int count= m_ctlList.GetItemCount();
	if (item > count )
	{
		//item = count;
	}

    CString stDate;
    if(m_ctlSelectDate.GetCurSel()>=0)
    {
        m_nSelectDate = m_ctlSelectDate.GetCurSel();
    }
    m_ctlSelectDate.GetLBText(m_nSelectDate, stDate);

	if (item >= count)
	{
		int tmp = m_arrTasks.GetSize()-1;
		if (CheckShow(m_arrTasks[tmp], stDate))
		{
			m_arrIndexMap.Add(tmp);
			SetDataToListCtrl(item, TRUE);
		}
		return true;
	}
	SetDataToListCtrl(item, FALSE);
	return true;
}

void CMyDlg::OnClickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
// 	DWORD dwPos = GetMessagePos();
// 	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
// 	
// 	m_ctlList.ScreenToClient(&point);
// 	
// 	LVHITTESTINFO lvinfo;
// 	lvinfo.pt = point;
// 	lvinfo.flags = LVHT_ABOVE;
// 	
// 	int nItem = m_ctlList.SubItemHitTest(&lvinfo);
// 	if(nItem != -1)
// 	{
// 		m_stStatus.Format("★单击的是第%d行第%d列", lvinfo.iItem + 1, lvinfo.iSubItem + 1);	
// 	}
// 
// 	UINT nFlag;
// 	nItem = m_ctlList.HitTest(point, &nFlag);
// 	if(nFlag == LVHT_ONITEMSTATEICON)
// 	{
// 		m_stStatus += "[点在listctrl的checkbox上]";
// 	} 
// 
// 
// 	UpdateData(FALSE);
// 	*pResult = 0;
}

void CMyDlg::OnColumnclickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
 	//CString st;
 	//st.Format("ItemId=%d\nSubId=%d",pNMListView->iItem, pNMListView->iSubItem);
 	//MessageBox(st);
	if (pNMListView->iSubItem == 0)
	{
		CheckAll();
	}
    else
    {
        (void)SortBySubItemAndRedraw(pNMListView->iSubItem);
    }
	*pResult = 0;
}

void CMyDlg::OnRclickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
    
		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_MENU_MAIN ) );
		CMenu* popup = menu.GetSubMenu(1);
		ASSERT( popup != NULL );

		MENUINFO mi;
		mi.cbSize = sizeof(MENUINFO);
		mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND; //子菜单也变色
		HBRUSH br = CPubData::gBr.operator HBRUSH(); // CreateSolidBrush(RGB(173,201,243));
		mi.hbrBack = br;
		//CMenu* pMenu = GetMenu();//但一定要在资源里定义一个菜单并调用,不能用工程自动生成的一个菜单
		SetMenuInfo(menu.m_hMenu,&mi);
		DrawMenuBar();

		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	} 
	*pResult = 0;

}

bool CMyDlg::WriteFileHead( FILE*fp, const FileHead& fileHead )
{
	if (NULL == fp)
		return false;
	
	static UINT8 data[SIZEOF16(FileHead)]={0};
	static UINT8 edata[SIZEOF16(FileHead)]={0};
	memcpy(data, &fileHead, sizeof(FileHead));
	EncryptData(data, edata, SIZEOF16(FileHead));
	fwrite(edata, SIZEOF16(FileHead), 1, fp);
	
	return true;
}

bool CMyDlg::WriteFileRecord( FILE*fp, const TaskMsg& task )
{
	if (NULL == fp)
		return false;
	
	static UINT8 data[SIZEOF16(TaskMsg)]={0};
	static UINT8 edata[SIZEOF16(TaskMsg)]={0};
	memcpy(data, &task, sizeof(TaskMsg));
	EncryptData(data, edata, SIZEOF16(TaskMsg));
	fwrite(edata, SIZEOF16(TaskMsg), 1, fp);
	
	return true;
}

bool CMyDlg::LoadFileHead( FILE*fp, FileHead& fileHead )
{
	if (NULL == fp)
		return false;
	
	static UINT8 data[SIZEOF16(FileHead)];
	static UINT8 edata[SIZEOF16(FileHead)];

	int ret = fread(data, 1, SIZEOF16(FileHead), fp);
	if (ret < SIZEOF16(FileHead))
	{
		CString st;
		st.Format("File Read Error![%d,%d]",GetLastError(), ferror(fp));
		//::MessageBox(st);
		return false;
	}
	EncryptData(data, edata, SIZEOF16(FileHead));
	
	memcpy(&fileHead, edata, sizeof(FileHead));
	//CString st;
	//st.Format("Count=%d",fileHead.RecordCount);
	//::MessageBox(st);
	return true;
}

bool CMyDlg::LoadFileRecord( FILE*fp, tagTaskMsg& task )
{
	if (NULL == fp)
		return false;
	
	static UINT8 data[SIZEOF16(TaskMsg)]={0};
	static UINT8 edata[SIZEOF16(TaskMsg)]={0};

	//fread(data, SIZEOF16(TaskMsg), 1, fp);
	//EncryptData(data, edata, SIZEOF16(TaskMsg));
	//memcpy(&task, edata, sizeof(TaskMsg));

	int ret = fread(data, 1, m_objFileHead.RecordLen, fp);
	if (ret < m_objFileHead.RecordLen)
	{
		CString st;
		st.Format("Load Head Error: %d %d ret = %d RecLen=%d",GetLastError(), ferror(fp), ret, m_objFileHead.RecordLen);
		//::MessageBox(st);
		return false;
	}
	EncryptData(data, edata, m_objFileHead.RecordLen);
	int a = m_objFileHead.RecordLen,b=sizeof(TaskMsg);
    TaskMsgTime32 task32;
    if (m_objFileHead.RecordLen == SIZEOF16(TaskMsg))
    {
        memcpy(&task, edata, sizeof(TaskMsg));
    }
    else //if(m_objFileHead.RecordLen == sizeof(TaskMsgTime32))
    {
        memcpy(&task32, edata, sizeof(TaskMsgTime32));
        task.CopyFrom32(task32);
    }
	
	if (ret < m_objFileHead.RecordLen)
	{
		CString st;
		st.Format("Title=[%s]",task.stTitle);
		//::MessageBox(st);
	}
	return true;
}

void CMyDlg::ChangeView( bool isBak = true)
{
	if (isBak)
	{
		m_ctlList.ShowWindow(SW_HIDE);
		m_ctlListTmp.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ctlListTmp.ShowWindow(SW_HIDE);
		m_ctlList.ShowWindow(SW_SHOW);
	}
}

void CMyDlg::CheckAll()
{
	static bool bIsCheck=false;
    SetColumnSortFlag(m_iSortIndex, SF_NO_FLAG);
    m_iSortIndex = -1;

	if (0 == m_ctlList.GetItemCount())
	{
		m_stStatus = "★没有数据！";
		UpdateData(FALSE);
		return;
	}

	int i=0;
	if (bIsCheck)
	{
		for (i=0; i<m_ctlList.GetItemCount(); i++)
		{
			m_ctlList.SetCheck(i, FALSE);
		}
        SetColumnSortFlag(0, SF_DESC_FLAG);
	}
	else
	{
		for (i=0; i<m_ctlList.GetItemCount(); i++)
		{
			m_ctlList.SetCheck(i);
		}
        SetColumnSortFlag(0, SF_ASC_FLAG);
	}
	bIsCheck = !bIsCheck;
}

/**
*	@brief	
*	  
*	@param viewIndex [in], 在视图中的下标
*	@param IsAdd [in], 是否是添加
*	@return void
*/
void CMyDlg::SetDataToListCtrl( int viewIndex, bool IsAdd/*=FALSE*/ )
{
	CString st;
	CTime tiTmp;
	int i = m_arrIndexMap.GetAt(viewIndex);
	st.Format("%-2d", viewIndex + 1);
	int nRow = viewIndex;
	if (IsAdd)
	{
		nRow = m_ctlList.InsertItem(m_ctlList.GetItemCount(), LPCTSTR(st));// 插入行
	}
	else
	{
		m_ctlList.SetItemText(nRow, 0, LPCTSTR(st));// 设置数据
	}

	m_ctlList.SetItemText(nRow, 1, m_arrTasks[i].stTitle);// 设置数据
		
	tiTmp = m_arrTasks[i].tiStartSet;
	st = tiTmp.Format("%Y-%m-%d %H:%M");
	m_ctlList.SetItemText(nRow, 2, LPCTSTR(st));
		
	tiTmp = m_arrTasks[i].tipNext.tiEndSet;
	st = tiTmp.Format("%Y-%m-%d %H:%M");
	m_ctlList.SetItemText(nRow, 3, LPCTSTR(st));
		
	st.Format("	%d%%",m_arrTasks[i].nPercent);
	m_ctlList.SetItemText(nRow, 4, LPCTSTR(st));
	
	tiTmp = m_arrTasks[i].tiEnd;
	if (tiTmp != 0)
	{
		st = tiTmp.Format("%Y-%m-%d %H:%M");
	}
	else
	{
		st.Empty();
	}
	m_ctlList.SetItemText(nRow, 5, LPCTSTR(st));
	m_ctlList.SetItemText(nRow, 6, m_arrTasks[i].stDetails);
}

bool CMyDlg::SetTaskToExcel( int iTaskIndex, int iSheetIndex, COleSafeArray& saRet )
{
	long   index[2]; 
	int i = iTaskIndex -1;
	index[0] = iSheetIndex - 1; 
	CString st;
	BSTR   bstr;
	CTime tiTmp;

	index[1] = 0; 
	st.Format("%-2d", i + 1);
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 1; 
	st = m_arrTasks[i].stTitle;
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 2; 
	tiTmp = m_arrTasks[i].tiStartSet;
	st = tiTmp.Format("%Y-%m-%d %H:%M");
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 3; 
	tiTmp = m_arrTasks[i].tipNext.tiEndSet;
	st = tiTmp.Format("%Y-%m-%d %H:%M");
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 4; 
	st.Format("	%d%%",m_arrTasks[i].nPercent);
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 5; 
	tiTmp = m_arrTasks[i].tiEnd;
	if (tiTmp != 0)
	{
		st = tiTmp.Format("%Y-%m-%d %H:%M");
	}
	else
	{
		st.Empty();
	}
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	index[1] = 6; 
	st = m_arrTasks[i].stDetails;
	bstr   =   st.AllocSysString(); 
	saRet.PutElement(index,bstr); //把st中的内容放入saRet   	
	SysFreeString(bstr);

	return true;
}

CString CMyDlg::GetDate(time_t ti)
{
	return CTime(ti).Format("%Y-%m-%d");
}

int CMyDlg::RebuildDate()
{
	int num=2;
	int i,j;
	time_t tiTmp;
	bool bFound;

	// 清除原来的表
	while (m_ctlSelectDate.GetCount()>0)
	{
		m_ctlSelectDate.DeleteString(0);
	}
	m_arrDateList.RemoveAll();
	m_ctlSelectDate.AddString("所有");
	m_ctlSelectDate.AddString("今日");
    m_ctlSelectDate.AddString("未完成");

	for(i=0; i<m_arrTasks.GetSize(); i++)
	{
		tiTmp = GetDateInt(m_arrTasks[i].tiStartSet);
		bFound = false;
		for(j=0; j<m_arrDateList.GetSize(); j++)
		{
			if(m_arrDateList[j] == tiTmp)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			j = tiTmp;
			m_arrDateList.Add(j);
			m_ctlSelectDate.AddString(GetDate(tiTmp));
		}
	}
	return num;
}

void CMyDlg::OnSelchangeComboSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if ( m_nSelectDate != m_nLastDateSel)
	{
        //CString st;
        //st.Format("%d -> %d",m_nLastDateSel, m_nSelectDate);
        //::MessageBox(st);
		RedrawListCtrl();
		m_nLastDateSel = m_nSelectDate;
		if((IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount()==DEFAULT_SELECT_DATE+1) && m_ctlList.GetItemCount()>0)
			RebuildTimeShaft();
		else
			ClearTimeShaft();
	}
}

bool CMyDlg::DoOpen( const CString& stFileName )
{
	FILE* fp = NULL;
	
	fp = fopen(LPCTSTR(stFileName), "rb");
	
	if (NULL == fp)
		return false;
// 	CString st;
// 	st.Format("打开DoOpen[%s]",LPCTSTR(stFileName));
// 	::MessageBox(st);
	m_arrTasks.RemoveAll();
	TaskMsg task;
	if (!LoadFileHead(fp, m_objFileHead))
	{
		m_stStatus.Format("读取打开文件[%s]失败！",LPCTSTR(stFileName));
		::MessageBox(m_stStatus,"读取失败", MB_ICONERROR);
		UpdateData(FALSE);
		return false;
	}
// 	CString st;
// 	st.Format("[m_objFileHead]\nHeadLen: \t%d\nRecordCount: \t%d\nRecordLen: \t%d\nVersion:  \t%d\n", 
// 		m_objFileHead.HeadLen, m_objFileHead.RecordCount, m_objFileHead.RecordLen, m_objFileHead.Version);
// 	MessageBox(st);
	
	CString stTmp=m_stStatus;
	for (size_t i=0; i<m_objFileHead.RecordCount; i++)
	{
		m_stStatus.Format("★正在打开文件[%d%%]...",i*100/m_objFileHead.RecordCount);
        if (!LoadFileRecord(fp, task))
        {
            m_stStatus.Format("Read Record[%d] Error!", i+1);
            UpdateData(FALSE);
            return false;
        }
// 		st.Format("[Task%02d]\nTitle: %s\nDetails: %s", i, task.stTitle, task.stDetails);
// 		MessageBox(st);
		m_arrTasks.Add(task);
		UpdateData(FALSE);
	}
	m_stStatus = stTmp;
	RedrawListCtrl();
	RebuildDate();
	m_ctlSelectDate.SetCurSel(0);
	UpdateData(FALSE);
	return true;
}

bool CMyDlg::DoOpenDfTasks(const CString& stFileName )
{
	FILE* fp = NULL;
	
	fp = fopen(LPCTSTR(stFileName), "rb");
	
	if (NULL == fp)
		return false;
	
	CPubData::gArrDfTasks.DelAll();
	TaskMsg task;
	FileHead tmpHead = m_objFileHead;

	if (!LoadFileHead(fp, m_objFileHead))
	{
		m_stStatus.Format("读取打开文件[%s]失败！",LPCTSTR(stFileName));
        ADD_ERROR(LPCTSTR(m_stStatus));
		::MessageBox(m_stStatus,"读取失败", MB_ICONERROR);
		UpdateData(FALSE);
		m_objFileHead = tmpHead;
		return false;
	}
	// CString st;
	// 	st.Format("[m_objFileHead]\nHeadLen: \t%d\nRecordCount: \t%d\nRecordLen: \t%d\nVersion:  \t%d\n", 
	// 		m_objFileHead.HeadLen, m_objFileHead.RecordCount, m_objFileHead.RecordLen, m_objFileHead.Version);
	// 	MessageBox(st);
    ADD_TRACE("读取默认任务[%d]条：", m_objFileHead.RecordCount);
	
	CString stTmp=m_stStatus;
	for (size_t i=0; i<m_objFileHead.RecordCount; i++)
	{
		m_stStatus.Format("★正在打开文件[%d%%]...",i*100/m_objFileHead.RecordCount);
        if (!LoadFileRecord(fp, task))
        {
            m_stStatus.Format("Read Record[%d] Error!", i+1);
            UpdateData(FALSE);
            return false;
        }
        ADD_TRACE("任务[%s]下一次提醒时间[%s]", task.stTitle, LPCTSTR(GetDateTime(task.tiNextRmd)));

		// 		st.Format("[Task%02d]\nTitle: %s\nDetails: %s", i, task.stTitle, task.stDetails);
		// 		MessageBox(st);
		CPubData::gArrDfTasks.AddTask(task);
		UpdateData(FALSE);
	}
	m_stStatus = stTmp;
	UpdateData(FALSE);
	CPubData::gArrDfTasks.SetChange(false);
	m_objFileHead = tmpHead;
	return true;
}

bool CMyDlg::DoSave( const CString& stFileName )
{
	UINT16 chkSum=0;
	int i;
	FILE* fp = NULL;

	fp = fopen(LPCTSTR(stFileName), "wb");
	
	if (NULL == fp)
		return false;
	
	FileHead head;
	head.SetKey(KEY);
	head.RecordCount = m_arrTasks.GetSize();
	head.RecordLen = SIZEOF16(TaskMsg);
	for (i=0; i<m_arrTasks.GetSize(); i++)
	{
		chkSum += m_arrTasks[i].GetCheckSum();
	}
	head.CheckSum = chkSum ^ 0xffff;
	// 	CString st;
	// 	st.Format("chkSum=%x,head.CheckSum=%x", chkSum, head.CheckSum);
	// 	MessageBox(st);
	head.HeadCheckSum = head.GetCheckSum() ^ 0xffff;
	WriteFileHead(fp, head);
	
	for (i=0; i<m_arrTasks.GetSize(); i++)
	{
		WriteFileRecord(fp, m_arrTasks[i]);
	}
	fclose(fp);
	return true;
}

bool CMyDlg::DoSaveDfTasks(const CString& stFileName )
{
	UINT16 chkSum=0;
	int i;
	FILE* fp = NULL;
	
	fp = fopen(LPCTSTR(stFileName), "wb");
	
	if (NULL == fp)
		return false;
	
	FileHead head;
	head.SetKey(KEY);
	head.RecordCount = CPubData::gArrDfTasks.GetCount();
	head.RecordLen = SIZEOF16(TaskMsg);
	int Count = CPubData::gArrDfTasks.GetCount();
	for (i=0; i<Count; i++)
	{
		chkSum += CPubData::gArrDfTasks[i].GetCheckSum();
	}
	head.CheckSum = chkSum ^ 0xffff;
	// 	CString st;
	// 	st.Format("chkSum=%x,head.CheckSum=%x", chkSum, head.CheckSum);
	// 	MessageBox(st);
	head.HeadCheckSum = head.GetCheckSum() ^ 0xffff;
	WriteFileHead(fp, head);
	
	for (i=0; i<Count; i++)
	{
		WriteFileRecord(fp, CPubData::gArrDfTasks[i]);
	}
	fclose(fp);
	return true;
}


void CMyDlg::OnRealExit()
{
	UpdateData(TRUE);
	CString stTmp;

    KillTimer(TIMER_REMIND);
    KillTimer(TIMER_SAVE);

	// 退出前保存普通任务数据
	if(m_bIsChanged)
	{
		stTmp = m_stCurFileName;
		if (stTmp.IsEmpty())
		{
			stTmp = CPubData::setMsg.GetDefaultFileName();
		}
		if (stTmp.IsEmpty())
		{
			::MessageBox("没有打开文件！\r\n也没有默认文件！所以不会进行保存！\n请先指定要保存的文件或者在设置中指定默认文件！"
                "", MB_ICONWARNING);
			return;
		}
		else if (!DoSave(stTmp))
		{
			m_stStatus.Format("保存文件[%s]失败！", LPCTSTR(stTmp));
			::MessageBox(m_stStatus,"保存失败", MB_ICONERROR);
            ADD_ERROR(m_stStatus);
		}
		else
		{
			m_stStatus.Format("保存文件[%s]成功！", LPCTSTR(stTmp));
            ADD_NORMAL(m_stStatus);
		}
	}

	// 退出前保存定时任务数据
	if (CPubData::gArrDfTasks.IsChanged())
	{
		stTmp = CPubData::GetDataFileName(FILE_NAME_DF_TASK);
		if (!DoSaveDfTasks(stTmp))
		{
			m_stStatus.Format("保存文件[%s]失败！", LPCTSTR(stTmp));
			::MessageBox(m_stStatus,"保存失败", MB_ICONERROR);
		}
		else
		{
			m_stStatus.Format("保存文件[%s]成功！", LPCTSTR(stTmp));
		}
	}
	UpdateData(FALSE);

	WriteOperatorLog("程序正常退出！谢谢使用！");
	::Shell_NotifyIcon(NIM_DELETE,&m_tnid);
	CDialog::OnOK();
}

CMyDlg::~CMyDlg()
{
	m_myBitmap.DeleteObject();
}

LRESULT CMyDlg::OnNotifyTray( WPARAM wParam,LPARAM lParam )
{
	switch(lParam) {
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		// Load main window here
		ShowWindow(SW_SHOW);
		break;
	case WM_RBUTTONUP:
		{
			CMenu menu; 
			menu.LoadMenu(IDR_MENU_TRAY); //载入事先定义的选单 
			CMenu*pMenu=menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos);
			MENUINFO mi;
			mi.cbSize = sizeof(MENUINFO);
			mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND; //子菜单也变色
			HBRUSH br = CPubData::gBr.operator HBRUSH(); // CreateSolidBrush(RGB(173,201,243));
			mi.hbrBack = br;
			// CMenu* pMenu = GetMenu();//但一定要在资源里定义一个菜单并调用,不能用工程自动生成的一个菜单
			SetMenuInfo(menu.m_hMenu,&mi);
			DrawMenuBar();
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());
		}
		break;
		case NIN_BALLOONUSERCLICK:
			// MessageBox("Click");
			break;
	default:
		return FALSE;
	}
	return TRUE;
}

void CMyDlg::RebuildTimeShaft()
{
	int i = 0, j = 0, tmp = 0;
	ColorNode tmpNode1,tmpNode2;
	m_arrTimeNode.RemoveAll();
	for(i=0; i<720; i++)
	{
		m_nColorPt[i] = CO_NO_TASK;
	}
	tmpNode1.bIsOdd = false;
	tmpNode1.nPos = 0;
	m_arrTimeNode.Add(tmpNode1);
	tmpNode2.bIsOdd = false;
	tmpNode2.nPos = 720;
	m_arrTimeNode.Add(tmpNode2);
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		j = 0;
		tmp = (m_arrTasks[m_arrIndexMap[i]].tiStartSet+28800)%86400 / 60;
		tmpNode1.bIsOdd = (tmp % 2 != 0);
		tmp >>= 1;
		if(tmpNode1.bIsOdd)
		{
			tmp++;
		}
		while(j<m_arrTimeNode.GetSize())
		{
			if(m_arrTimeNode[j].nPos> tmp)
				break;
			j++;
		}
		tmpNode1.nPos = tmp;
		m_arrTimeNode.InsertAt(j, tmpNode1);
		j = 0;
		tmp = (m_arrTasks[m_arrIndexMap[i]].tipNext.tiEndSet+28800)%86400 / 60;
		tmpNode2.bIsOdd = (tmp % 2 != 0);
		tmp >>= 1;
		if(tmpNode2.bIsOdd)
		{
			tmp++;
		}
		while(j<m_arrTimeNode.GetSize())
		{
			if(m_arrTimeNode[j].nPos>tmp)
				break;
			j++;
		}
		tmpNode2.nPos = tmp;
		m_arrTimeNode.InsertAt(j, tmpNode2);

		for(j=tmpNode1.nPos; j<tmpNode2.nPos; j++)
		{
			m_nColorPt[j] = CO_NOT_STARTED;
		}
	}

	int tmp2 = 0;
	bool tmpOdd = false;

	// 检查已经开始的
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_arrTasks[m_arrIndexMap[i]].nPercent > 0)
		{
			tmp = (m_arrTasks[m_arrIndexMap[i]].tiStartSet+28800)%86400 / 60;
			tmpOdd = (tmp % 2 != 0);
			tmp >>= 1;
			if(tmpOdd)
			{
				tmp++;
			}
			tmp2 = (m_arrTasks[m_arrIndexMap[i]].tipNext.tiEndSet+28800)%86400 / 60;
			tmpOdd = (tmp2 % 2 != 0);
			tmp2 >>= 1;
			if(tmpOdd)
			{
				tmp2++;
			}
			for(j=tmp; j<tmp2; j++)
			{
				m_nColorPt[j] = CO_STARTED;
			}
		}
	}

	// 检查完成的
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_arrTasks[m_arrIndexMap[i]].tiEnd > 0)
		{
			tmp = (m_arrTasks[m_arrIndexMap[i]].tiStartSet+28800)%86400 / 60;
			tmpOdd = (tmp % 2 != 0);
			tmp >>= 1;
			if(tmpOdd)
			{
				tmp++;
			}
			tmp2 = (m_arrTasks[m_arrIndexMap[i]].tipNext.tiEndSet+28800)%86400 / 60;
			tmpOdd = (tmp2 % 2 != 0);
			tmp2 >>= 1;
			if(tmpOdd)
			{
				tmp2++;
			}
			for(j=tmp; j<tmp2; j++)
			{
				m_nColorPt[j] = CO_FINISHED;
			}
		}
	}
	
	CRect rect(m_objRect);
	rect.left -= 20;
	rect.top -= 20;
	rect.right += 20;
	rect.bottom += 50;
	InvalidateRect(&rect, TRUE);
}

// void CMyDlg::RebuildTimeShaft()
// {
// 	int tmp;
// 	tmp = 0;
// 	m_arrTimeNode.Add(tmp);
// 	tmp = CO_FREE;
// 	m_arrTimeColor.Add(tmp);
// 	tmp = 720;
// 	m_arrTimeNode.Add(tmp);
// 	tmp = CO_NOT_INC;
// 	m_arrTimeColor.Add(tmp);
// 	int i,j,k, start;
// 	for(i=0; i<m_arrTasks.GetSize(); i++)
// 	{
// 		j = 0;
// 		while(j<m_arrTimeNode.GetSize())
// 		{
// 			if(m_arrTimeNode[j]>(m_arrTasks[i].tiStartSet+28800)%86400 / 60)
// 				break;
// 			j++;
// 		}
// 		if (m_arrTimeColor[j-1] == CO_FREE)
// 		{
// 			tmp = CO_USED;
// 			m_arrTimeColor.InsertAt(j, tmp); 
// 		}
// 		else if (m_arrTimeColor[j-1] == CO_USED)
// 		{
// 			tmp = CO_REDUPLICATE;
// 			m_arrTimeColor.InsertAt(j, tmp);
// 		}
// 		start = j;
// 		tmp = (m_arrTasks[i].tiStartSet+28800)%86400 / 60;
// 		m_arrTimeNode.InsertAt(j, tmp);
// 		j = 0;
// 		while(j<m_arrTimeNode.GetSize())
// 		{
// 			if(m_arrTimeNode[j]>(m_arrTasks[i].tipNext.tiEndSet+28800)%86400 / 60)
// 				break;
// 			j++;
// 		}
// 		for(k=start+1; k<j; k++)
// 		{
// 			if (m_arrTimeColor[k] == CO_FREE)
// 			{
// 				tmp = CO_USED;
// 				m_arrTimeColor.SetAt(k,tmp); 
// 			}
// 			else if (m_arrTimeColor[k] == CO_USED)
// 			{
// 				tmp = CO_REDUPLICATE;
// 				m_arrTimeColor.InsertAt(j, tmp);
// 			}
// 		}
// 		if (m_arrTimeColor[j-1] == CO_FREE)
// 		{
// 			tmp = CO_FREE;
// 			m_arrTimeColor.InsertAt(j, tmp); 
// 		}
// 		else if (m_arrTimeColor[j-1] == CO_USED)
// 		{
// 			tmp = CO_FREE;
// 			m_arrTimeColor.InsertAt(j, tmp);
// 		}
// 		tmp = (m_arrTasks[i].tipNext.tiEndSet+28800)%86400 / 60;
// 		m_arrTimeNode.InsertAt(j, tmp);
// 	}
// }

void CMyDlg::ClearTimeShaft()
{
	int i;
	m_arrTimeNode.RemoveAll();
	for(i=0; i<720; i++)
	{
		m_nColorPt[i] = CO_NO_TASK;
	}
	CRect rect(m_objRect);
	rect.top -= 20;
	rect.bottom += 50;
	rect.left -= 20;
	rect.right += 20;
	InvalidateRect(&rect, TRUE);
}

void CMyDlg::OnMyShowWindow()
{
	ShowWindow(SW_SHOW);
}

BOOL CMyDlg::OnToolTipText( UINT NID, NMHDR* pNMHDR, LRESULT* pResult )
{
	  ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);  // UNICODE消息  
		TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;  
		TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR; 
		CString strTipText;
		// 获取控件ID
		UINT nID = pNMHDR->idFrom;
		if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) 
			||   pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))  
		{   
			// idFrom为工具条的HWND    
			nID = ::GetDlgCtrlID((HWND)nID);  
		}    
		if (nID != 0) //不为分隔符  
		{   
			strTipText.LoadString(nID);   
			strTipText = strTipText.Mid(strTipText.Find('\n',0)+1);
			// START add by sun.hualiang on 2012.9.17 for 添加普通控件的提示
			if (strTipText.IsEmpty())
			{
				if(!m_mapTip.Lookup(nID,strTipText))
					return FALSE;
			}
			// END add by sun.hualiang on 2012.9.17
#ifndef _UNICODE   
			if (pNMHDR->code == TTN_NEEDTEXTA)   
			{
				lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));   
			}   
			else   
			{    
				_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));   
			} 
#else   
			if (pNMHDR->code == TTN_NEEDTEXTA)   
			{    
				_wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));   
			}   
			else   
			{    
				lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));   
			} 
#endif
			*pResult = 0;      

			// 使工具条提示窗口在最上面   
			::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
				SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); 
		}
		return TRUE;
}

void CMyDlg::SetTips()
{
	m_mapTip.SetAt(IDC_COMBO_SELECT, "选择日期");
}

BOOL CMyDlg::PreTranslateMessage( MSG* pMsg )
{
		// TODO: Add your specialized code here and/or call the base class
// 		CToolTipCtrl m_openToolTip;
// 
// 		switch(pMsg->message)
// 		{ 
// 		case WM_LBUTTONDOWN: 
// 		case WM_LBUTTONUP: 
// 		case WM_MOUSEMOVE: 
// 			m_openToolTip.RelayEvent(pMsg); 
// 		} 
		return CDialog::PreTranslateMessage(pMsg);
}

void CMyDlg::OnNotDefined()
{
	::MessageBox("此功能暂时没有完成\r\n敬请期待！", "Sorry", MB_ICONINFORMATION);
}

void CMyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TIMER_REMIND:
			{
				ModifyTime();
#if 1
                //if(CPubData::pRmdLock->IsLocked())
                //{
                //    return;
                //}
                class _TRmdLock
                {
                public:
                    BOOL m_bLocked;
                    CMyDlg * m_pDlg;
                    _TRmdLock()
                    {
                        if (CPubData::pRmdLock->IsLocked())
                        {
                            m_bLocked = false;
                        }
                        else
                        {
                            m_bLocked = CPubData::pRmdLock->Lock(200);
                        }
                        if (m_bLocked)
                        {
                            ++ CPubData::oLockCounter.nLockSuccess;
                        }
                        else
                        {
                            ++ CPubData::oLockCounter.nLockFailed;
                        }
                    }
                    ~_TRmdLock()
                    {
                        if (m_bLocked)
                        {
                            CPubData::pRmdLock->Unlock();
                            ++ CPubData::oLockCounter.nUnlock;
                        }
                    }
                }_lock;

                if (!_lock.m_bLocked)
                {
                    return;
                }
				CheckTaskRemind();
                CheckDfTaskRemind();
#endif
			}
			break;
		case TIMER_SAVE:
			UpdateData();
			if(m_bIsChanged)
			{
				if(!m_stCurFileName.IsEmpty())
				{
					if(DoSave(m_stCurFileName))
					{
						m_stStatus.Format("文件[%s]于[%s]自动保存成功！", LPCTSTR(m_stCurFileName), LPCTSTR(GetTime(time(NULL))));
						SetChange(false);
					}
					else
					{
						m_stStatus.Format("文件[%s]于[%s]自动保存失败！", LPCTSTR(m_stCurFileName), LPCTSTR(GetTime(time(NULL))));
					}
				}
			}
			if(CPubData::gArrDfTasks.IsChanged())
			{
				if(DoSaveDfTasks(CPubData::GetDataFileName(FILE_NAME_DF_TASK)))
				{
					m_stStatus.Format("文件[%s]于[%s]自动保存成功！", LPCTSTR(m_stCurFileName), LPCTSTR(GetTime(time(NULL))));
					CPubData::gArrDfTasks.SetChange(false);
				}
				else
				{
					m_stStatus.Format("文件[%s]于[%s]自动保存失败！", LPCTSTR(m_stCurFileName), LPCTSTR(GetTime(time(NULL))));
				}
			}
			UpdateData(FALSE);
			break;
		case TIMER_TRAY_ICON:
			//DoRedrawTrayIcon();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMyDlg::CheckTaskRemind()
{
	int i=0;
	time_t tiNow = time(NULL);
	for(i=0; i<m_arrTasks.GetSize(); i++)
	{
		// 开始提醒
		if (m_arrTasks[i].tiStartSet <= tiNow && !m_arrTasks[i].nIsTipStarted)
		{
			m_arrTasks[i].nIsTipStarted = 1;
            if (m_bIsOnStart) // 开始时批量处理
            {
                m_arrTimeoutTasks.Add(&(m_arrTasks[i]));
            }
            else
            {
                m_nTipIndex = i;
                m_nTipType = TT_CO_START;
                if (!DoTaskCommandImpl())
                {
                    return;
                }
            }
		}

		// 额外提醒
		if (m_arrTasks[i].tiExtraRmd <= tiNow && !(m_arrTasks[i].nIsTipExtra))
		{
			m_arrTasks[i].nIsTipExtra = 1;
            if (m_bIsOnStart) // 开始时批量处理
            {
                m_arrTimeoutTasks.Add(&(m_arrTasks[i]));
            }
            else
            {
                m_nTipIndex = i;
                m_nTipType = TT_CO_EXTRA;
                if (!DoTaskCommandImpl())
                {
                    return;
                }
            }
		}

		// 结束提醒
		if (m_arrTasks[i].tipNext.tiEndSet <= tiNow && !(m_arrTasks[i].nIsTipEnded))
		{
			m_arrTasks[i].nIsTipEnded = 1;
            if (m_bIsOnStart) // 开始时批量处理
            {
                m_arrTimeoutTasks.Add(&(m_arrTasks[i]));
            }
            else
            {
                m_nTipIndex = i;
                m_nTipType = TT_CO_END;
                if (!DoTaskCommandImpl())
                {
                    return;
                }
            }
		}
	}
}

void CMyDlg::SetChange(bool bIsChanged)
{
	m_bIsChanged = bIsChanged;
	CStatic* pChg = (CStatic*)GetDlgItem(IDC_STATIC_CHANGE);
	if (bIsChanged)
	{
		pChg->EnableWindow(TRUE);
		pChg->SetWindowText("已修改");
	}
	else
	{
		pChg->EnableWindow(FALSE);
		pChg->SetWindowText("未修改");
	}
}

void CMyDlg::OnUpdateMainSubmit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// GetDlgItem(pCmdUI->m_nID)->EnableWindow(FALSE);
}

HBRUSH CMyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
// 	if (pWnd == this)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return CPubData::gBr;
// 	}
// 	else
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
// 	}
	switch (nCtlColor)
	{
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,80,0));
		return CPubData::gBr;
		break;
	}
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
	{
		pDC->SetBkMode(TRANSPARENT);
		return CPubData::gBr;
	}
	case CTLCOLOR_DLG:
	{
		if(pWnd==this)
		{
			return CPubData::gBr;
		}
	}
	default:
		return hbr;
	}
	
	// TODO: Return a different brush if the default is not desire
	return hbr;
}

bool CMyDlg::DoRemind( TaskMsg& task, int & type, int & mins )
{
	//SetTimer(TIMER_TRAY_ICON, 500, NULL);
	CMsgHandleDlg dlg(task, this);
	if(dlg.DoModal() == IDOK)
	{
			// 结束提醒或设置再次提醒
			type = dlg.GetRemindType(&mins);
			if (dlg.IsSubTask())
			{
				task.nIsTipEnded = 1;
				task.tiEnd = time(NULL);
				task.nPercent = 100;
			}
	}
	else
	{
			// 自动设置再次提醒
			type = RMD_5_MIN;
			mins = 5;
	}
	//KillTimer(TIMER_TRAY_ICON);
	//DoRedrawTrayIcon(true);
	return true;
}

bool CMyDlg::DoTaskCommand( const TaskMsg& tsk )
{
    CString st;
    st.Format("任务[%s]确定执行命令[%s]?", tsk.stTitle, tsk.stCmd);
    CString stRealCmd = TConfig::GetCmdByName(tsk.stCmd);
    if (stRealCmd.IsEmpty())
    {
        stRealCmd = tsk.stCmd;
    }
    if (CheckMsgTypeRemind(tsk.nMsgType) 
        && ::MessageBox(st, "命令确认", MB_ICONINFORMATION, 10, FALSE) != IDOK)
    {
        ADD_NORMAL("任务[%s]命令[%s]被取消执行！", tsk.stTitle, tsk.stCmd);
        return true;
    }
    else
    {
        UINT iRet = ::WinExec(stRealCmd, SW_HIDE);
        if (0 == iRet)
        {
            ADD_ERROR("任务[%s]执行命令[%s]失败：系统内存或资源已耗尽", tsk.stTitle, stRealCmd);
            return false;
        }
        if (iRet>31)
        {
            ADD_NORMAL("任务[%s]执行命令[%s]成功！", tsk.stTitle, tsk.stCmd);
            return true;
        }
        else
        {
            LPVOID lpMsgBuf = NULL;
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER
                |FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                (LPTSTR) &lpMsgBuf,
                0,
                NULL
                );
            ADD_ERROR("任务[%s]执行命令[%s]失败：%s", tsk.stTitle, stRealCmd, (const char *)lpMsgBuf);
            LocalFree( lpMsgBuf );
            return false;
        }
    }

    return true;
}

void CMyDlg::InitSomeTasks()
{
	TaskMsg task;
	CTime tiNow = CTime::GetCurrentTime();
	time_t ti = tiNow.GetTime()/3600*3600;
	int index = 3, index2=0;
	strcpy(task.stTitle, "第一次使用本软件");
	sprintf(task.stDetails, "欢迎使用本软件!\r\n我的任务 v%d.%d", VER_HIGH, VER_LOW);
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	index2++;
	task.nPercent = 100;
	task.tiEnd = ti-3600*(index-1)-200;
	index--;
	m_arrTasks.Add(task);

	strcpy(task.stTitle, "设置文件名和音乐");
	sprintf(task.stDetails, "点击设置进行设置!\r\n我的任务 v%d.%d", VER_HIGH, VER_LOW);
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	task.tiEnd = 0;
	task.nIsTipEnded = 1;
	index2++;
	task.nPercent = 0;
	index--;
	m_arrTasks.Add(task);

	strcpy(task.stTitle, "提交一条任务");
	sprintf(task.stDetails, "将第二条任务提交到100%%\r\n先选中第二条任务，然后：\r\n①右击鼠标可弹出菜单\r\n②可使用上方的工具栏按钮\r\n③也可以使用菜单栏的【编辑】");
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	index2++;
	task.nPercent = 0;
	task.tiEnd = 0;
	index--;
	m_arrTasks.Add(task);

	strcpy(task.stTitle, "编辑一条任务");
	sprintf(task.stDetails, "先选中一条任务，然后：\r\n①右击鼠标可弹出菜单\r\n②可使用上方的工具栏按钮\r\n③也可以使用菜单栏的【编辑】");
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	index2++;
	task.nPercent = 0;
	task.tiEnd = 0;
	index--;
	m_arrTasks.Add(task);

	strcpy(task.stTitle, "删除默认任务");
	sprintf(task.stDetails, "删除前四条任务\r\n先选中一条或多条任务，然后：\r\n①右击鼠标可弹出菜单\r\n②可使用上方的工具栏按钮\r\n③也可以使用菜单栏的【编辑】");
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	index2++;
	task.nPercent = 0;
	task.tiEnd = 0;
	index--;
	m_arrTasks.Add(task);

	strcpy(task.stTitle, "新增自己的任务");
	sprintf(task.stDetails, "也是使用菜单完成：\r\n①可使用上方的工具栏按钮\r\n②也可以使用菜单栏的【编辑】");
	task.tiStartSet = ti - 3600*index;
	task.nIsTipStarted = 1;
	task.tipNext.tiEndSet = ti-3600*(index-1);
	task.nIsTipEnded = 1;
	task.tiExtraRmd = tiNow.GetTime() + index2*60;
	task.nIsTipExtra = 0;
	index2++;
	task.nPercent = 0;
	task.tiEnd = 0;
	index--;
	m_arrTasks.Add(task);

	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	CPubData::setMsg.AddMusic(LPCTSTR(strPath+"\\Music\\卡农.wav"));
}

BOOL CMyDlg::UpdateData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (!bSaveAndValidate)
	{
		if (m_stStatus != m_stLastStatus)
		{
			if(m_stStatus.Find("★", 0) == 0)
			{
				// 不记录日志
			}
			else if(m_stStatus.Compare("就绪")==0)
			{
				// 不记录日志
			}
			else
			{
				WriteOperatorLog(m_stStatus);
			}
			// WriteOperatorLog(m_stStatus);
			m_stLastStatus = m_stStatus;
		}
	}
	return CDialog::UpdateData(bSaveAndValidate);
}

bool CMyDlg::WriteOperatorLog( const char* st )
{
	ADD_NORMAL(st);
    return true;
}

void CMyDlg::OnMainViewLog() 
{
	// TODO: Add your command handler code here
	CString stLogFile = CMyLog::GetLogPointer()->GetLogFileName();
	if (m_stOpenLogCmd.IsEmpty())
	{
		::ShellExecute(NULL,"open","notepad.exe",LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_SHOWDEFAULT);
	}
	else
	{
		int i = m_stOpenLogCmd.Find(".bat");
		int j = m_stOpenLogCmd.Find(".cmd");
		if ( (i>0 && i+4 == m_stOpenLogCmd.GetLength()) 
			|| (j>0 && j+4 == m_stOpenLogCmd.GetLength())  )
		{
			::ShellExecute(NULL,"open", LPCTSTR(m_stOpenLogCmd),LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_HIDE);
		}
		else
		{
			::ShellExecute(NULL,"open", LPCTSTR(m_stOpenLogCmd),LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_SHOWDEFAULT);
		}
	}
}

void CMyDlg::OnMainSetViewTool() 
{
	// TODO: Add your command handler code here
	CMyFileDialog dlg(TRUE, "可执行程序(*.exe)|*.exe|", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"可执行程序(*.exe)|*.exe|脚本(*.cmd;*.bat)|*.cmd;*.bat|Perl脚本(*.pl)|*.pl|所有文件(*.*)|*.*|",
		this);
	if (IDOK == dlg.DoModal())
	{
		m_stOpenLogCmd = dlg.GetPathName();
	}
}

void CMyDlg::OnExportCur()
{
	UpdateData(TRUE);
	CString cStrFile;
    if (!GetExportFileName(FT_EXCEL, cStrFile, m_stStatus, this))
    {
        UpdateData(FALSE);
        return;
    }

	COleVariant covTrue((short)TRUE),covFalse((short)FALSE),covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
  
	_Application app; //Excel程序   
	Workbooks books; //工作簿集合   
	_Workbook book;  //工作表   
	Worksheets sheets;  //工作簿集合   
	_Worksheet sheet; //工作表集合   
	Range range; //使用区域   
	
	CoUninitialize();  
	
	book.PrintPreview(_variant_t(false));  
	if(CoInitialize(NULL)==S_FALSE)   
	{  
		::MessageBox(_T("初始化COM支持库失败！"), "ERROR", MB_ICONERROR);
		m_stStatus.Format("导出当前视图数据到文件 [%s] 失败: 初始化COM支持库失败！",cStrFile);
		UpdateData(FALSE);
		return;  
	}  
  
	if(!app.CreateDispatch(_T("Excel.Application"))) //创建IDispatch接口对象   
	{  
		::MessageBox(_T("创建IDispatch接口对象失败！"), "失败", MB_ICONERROR);   
		m_stStatus.Format("导出当前视图数据到文件 [%s] 失败: 创建IDispatch接口对象失败！",cStrFile);
		UpdateData(FALSE);
		return;
	}  
  
	
	books = app.GetWorkbooks();  
	book = books.Add(covOptional);  
	
  
	sheets = book.GetWorksheets();  
	sheet = sheets.GetItem(COleVariant((short)1));  //得到第一个工作表
    CString stDate;
    m_ctlSelectDate.GetLBText(m_ctlSelectDate.GetCurSel(), stDate);
    sheet.SetName(LPCTSTR(stDate));
  
	CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   
  
	int   m_cols   = pmyHeaderCtrl-> GetItemCount(); //获取列数   
	int   m_rows = m_ctlList.GetItemCount();  //获取行数   
  
  
	TCHAR     lpBuffer[256];      
  
	HDITEM   hdi; //This structure contains information about an item in a header control. This structure has been updated to support header item images and order values.   
	hdi.mask   =   HDI_TEXT;  
	hdi.pszText   =   lpBuffer;  
	hdi.cchTextMax   =   256;   
  
	CString   colname;  
	CString strTemp;  
  
	int   iRow,iCol;  
	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL    
	{   
		GetCellName(iCol + 1 , 1, colname); //(colname就是对应表格的A1,B1,C1,D1)   
		range   =   sheet.GetRange(COleVariant(colname),COleVariant(colname));    
		pmyHeaderCtrl-> GetItem(iCol,   &hdi); //获取表头每列的信息   
		range.SetValue2(COleVariant(hdi.pszText));  //设置每列的内容   
		
		int   nWidth   =   m_ctlList.GetColumnWidth(iCol)/6;   
		//得到第iCol+1列      
		range.AttachDispatch(range.GetItem(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);     
		//设置列宽     
		range.SetColumnWidth(_variant_t((long)nWidth));  
	}   
  
  range   =   sheet.GetRange(COleVariant( _T("A1 ")),   COleVariant(colname));   
  range.SetRowHeight(_variant_t((long)50));//设置行的高度    
  range.SetVerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108    
  
  COleSafeArray   saRet; //COleSafeArray类是用于处理任意类型和维数的数组的类   
  DWORD   numElements[]={m_rows,m_cols};       //行列写入数组   
  saRet.Create(VT_BSTR,   2,   numElements); //创建所需的数组   
  
  range = sheet.GetRange(COleVariant( _T("A2 ")),covOptional); //从A2开始   
  range = range.GetResize(COleVariant((short)m_rows),COleVariant((short)m_cols)); //表的区域   
  
  long   index[2];    
  int t = m_ctlList.GetItemCount();
  for(   iRow   =   1;   iRow   <=   m_rows;   iRow++)//将列表内容写入EXCEL    
  {
		m_stStatus.Format("★正在导出数据[%d%%]...",iRow*100/t);
		UpdateData(FALSE);
		for   (iCol = 1;   iCol <= m_cols;   iCol++)    
		{   
			index[0]=iRow-1;   
			index[1]=iCol-1;   	
			CString   szTemp;   		
			szTemp=m_ctlList.GetItemText(iRow-1,iCol-1); //取得m_ctlList控件中的内容   
			BSTR   bstr   =   szTemp.AllocSysString(); //The AllocSysString method allocates a new BSTR string that is Automation compatible   
			saRet.PutElement(index,bstr); //把m_ctlList控件中的内容放入saRet   	
			SysFreeString(bstr);  
		}   
  }    
	range.SetValue2(COleVariant(saRet)); //将得到的数据的saRet数组值放入表格   
	book.SaveCopyAs(COleVariant(cStrFile)); //保存到cStrFile文件   
	book.SetSaved(true);  
	books.Close();
	//   
	m_stStatus.Format("导出当前视图数据到文件 [%s] 成功！",cStrFile);
	UpdateData(FALSE);
	book.ReleaseDispatch();  
	books.ReleaseDispatch();   
  
	app.ReleaseDispatch();  
	app.Quit();
}

void CMyDlg::GetCellName( int p1, int p2, CString& colname )
{
	colname.Format("%c%d",'A'+p1-1,p2);
}

void CMyDlg::OnButtonToolExport()
{
	CExportChooseDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		int type = dlg.GetExportType();
// 		CString st;
// 		st.Format("%d",type);
// 		MessageBox(st);
		int index = dlg.GetExportIndex();
		if (index == 0)
		{
			switch(type)
			{
			case ET_EXPORT_CUR:
				OnExportCur();
				break;
			case ET_EXPORT_ALL:
				OnExportAll();
				break;
			case ET_EXPORT_BY_DAY:
				OnExportAllByDay();
				break;
			default:
				break;
			}
		}
		else
		{
			switch(type)
			{
			case ET_EXPORT_CUR:
				OnExportCurTxt();
				break;
			case ET_EXPORT_ALL:
				OnExportAllTxt();
				break;
			case ET_EXPORT_BY_DAY:
				OnExportAllByDayTxt();
				break;
			default:
				break;
			}
		}
	}
	else
	{
		//MessageBox("取消导出");
	}
}

bool CMyDlg::OnExportAll()
{
	UpdateData(TRUE);
    CString cStrFile;
    if (!GetExportFileName(FT_EXCEL, cStrFile, m_stStatus, this))
    {
        UpdateData(FALSE);
        return false;
    }
	COleVariant covTrue((short)TRUE),covFalse((short)FALSE),covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
  
	_Application app; //Excel程序   
	Workbooks books; //工作簿集合   
	_Workbook book;  //工作表   
	Worksheets sheets;  //工作簿集合   
	_Worksheet sheet; //工作表集合   
	Range range; //使用区域   
	
	CoUninitialize();  
	
	book.PrintPreview(_variant_t(false));  
	if(CoInitialize(NULL)==S_FALSE)   
	{  
		::MessageBox(_T("初始化COM支持库失败！"), "失败", MB_ICONERROR);
		m_stStatus.Format("导出所有数据到文件 [%s] 失败: 初始化COM支持库失败！",cStrFile);
		UpdateData(FALSE);
		return false;  
	}  
  
	if(!app.CreateDispatch(_T("Excel.Application"))) //创建IDispatch接口对象   
	{  
		::MessageBox(_T("创建IDispatch接口对象失败！"), "失败", MB_ICONERROR);   
		m_stStatus.Format("导出所有数据到文件 [%s] 失败: 创建IDispatch接口对象失败！",cStrFile);
		UpdateData(FALSE);
		return false;
	}  
  
	
	books = app.GetWorkbooks();  
	book = books.Add(covOptional);  
	
  
	sheets = book.GetWorksheets();  
	sheet = sheets.GetItem(COleVariant((short)1));  //得到第一个工作表
    sheet.SetName("所有任务");
  
	CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   
  
	int   m_cols   = pmyHeaderCtrl-> GetItemCount(); //获取列数   
	int   m_rows = m_arrTasks.GetSize();  //获取行数   
  
  
	TCHAR     lpBuffer[256];      
  
	HDITEM   hdi; //This structure contains information about an item in a header control. This structure has been updated to support header item images and order values.   
	hdi.mask   =   HDI_TEXT;  
	hdi.pszText   =   lpBuffer;  
	hdi.cchTextMax   =   256;   
  
	CString   colname;  
	CString strTemp;  
  
	int   iRow,iCol;  
	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL    
	{   
		GetCellName(iCol + 1 , 1, colname); //(colname就是对应表格的A1,B1,C1,D1)   
		range   =   sheet.GetRange(COleVariant(colname),COleVariant(colname));    
		pmyHeaderCtrl-> GetItem(iCol,   &hdi); //获取表头每列的信息   
		range.SetValue2(COleVariant(hdi.pszText));  //设置每列的内容   
		
		int   nWidth   =   m_ctlList.GetColumnWidth(iCol)/6;   
		//得到第iCol+1列      
		range.AttachDispatch(range.GetItem(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);     
		//设置列宽     
		range.SetColumnWidth(_variant_t((long)nWidth));  
	}   
  
  range   =   sheet.GetRange(COleVariant( _T("A1 ")),   COleVariant(colname));   
  range.SetRowHeight(_variant_t((long)50));//设置行的高度    
  range.SetVerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108    
  
  COleSafeArray   saRet; //COleSafeArray类是用于处理任意类型和维数的数组的类   
  DWORD   numElements[]={m_rows,m_cols};       //行列写入数组   
  saRet.Create(VT_BSTR,   2,   numElements); //创建所需的数组   
  
  range = sheet.GetRange(COleVariant( _T("A2 ")),covOptional); //从A2开始   
  range = range.GetResize(COleVariant((short)m_rows),COleVariant((short)m_cols)); //表的区域   
     
  int t = m_ctlList.GetItemCount();
  for(   iRow   =   1;   iRow   <=   m_rows;   iRow++)//将列表内容写入EXCEL    
  {
		m_stStatus.Format("★正在导出数据[%d%%]...",iRow*100/t);
		UpdateData(FALSE);
		if(!SetTaskToExcel(iRow, iRow, saRet))
		{
			m_stStatus.Format("导出第[%d]行数据失败！",iRow);
			UpdateData(FALSE);
			return false;
		}
  }    
	range.SetValue2(COleVariant(saRet)); //将得到的数据的saRet数组值放入表格   
	book.SaveCopyAs(COleVariant(cStrFile)); //保存到cStrFile文件   
	book.SetSaved(true);  
	books.Close();
	//   
	m_stStatus.Format("导出所有数据到文件 [%s] 成功！",cStrFile);
	UpdateData(FALSE);
	book.ReleaseDispatch();  
	books.ReleaseDispatch();   
  
	app.ReleaseDispatch();  
	app.Quit();
	return true;
}

bool CMyDlg::OnExportAllByDay()
{
	UpdateData(TRUE);
    CString cStrFile;
    if (!GetExportFileName(FT_EXCEL, cStrFile, m_stStatus, this))
    {
        UpdateData(FALSE);
        return false;
    }
	COleVariant covTrue((short)TRUE),covFalse((short)FALSE),covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
  
	_Application app; //Excel程序   
	Workbooks books; //工作簿集合   
	_Workbook book;  //工作表   
	Worksheets sheets;  //工作簿集合   
	_Worksheet sheet; //工作表集合   
	Range range; //使用区域   
	
	CoUninitialize();  
	
	book.PrintPreview(_variant_t(false));  
	if(CoInitialize(NULL)==S_FALSE)   
	{  
		::MessageBox(_T("初始化COM支持库失败！"), "失败", MB_ICONERROR);
		m_stStatus.Format("导出所有数据到文件 [%s] 失败: 初始化COM支持库失败！",cStrFile);
		UpdateData(FALSE);
		return false;  
	}  
  
	if(!app.CreateDispatch(_T("Excel.Application"))) //创建IDispatch接口对象   
	{  
		::MessageBox(_T("创建IDispatch接口对象失败！"), "失败", MB_ICONERROR);   
		m_stStatus.Format("导出所有数据到文件 [%s] 失败: 创建IDispatch接口对象失败！",cStrFile);
		UpdateData(FALSE);
		return false;
	}  
  
	
	books = app.GetWorkbooks();  
	book = books.Add(covOptional);
	sheets = book.GetWorksheets();
	for(int i=sheets.GetCount(); i<m_ctlSelectDate.GetCount()-DEFAULT_SELECT_DATE; i++)
	{
		sheets.Add(covOptional, covOptional, covOptional, covOptional);
	}
    for(int index = DEFAULT_SELECT_DATE; index < m_ctlSelectDate.GetCount(); index++)
	{
		CString stDate;
		m_ctlSelectDate.GetLBText(index, stDate);
		
		sheet = sheets.GetItem(COleVariant((short)(index-1)));  //得到第index个工作表
		sheet.SetName(LPCTSTR(stDate));
		
		CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   
		
		int   m_cols   = pmyHeaderCtrl-> GetItemCount(); //获取列数   
		int   m_rows = m_arrTasks.GetSize();  //获取行数   
		
		
		TCHAR     lpBuffer[256];      
		
		HDITEM   hdi; //This structure contains information about an item in a header control. This structure has been updated to support header item images and order values.   
		hdi.mask   =   HDI_TEXT;  
		hdi.pszText   =   lpBuffer;  
		hdi.cchTextMax   =   256;   
		
		CString   colname;  
		CString strTemp;  
		
		int   iRow,iCol;  
		for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL    
		{   
			GetCellName(iCol + 1 , 1, colname); //(colname就是对应表格的A1,B1,C1,D1)   
			range   =   sheet.GetRange(COleVariant(colname),COleVariant(colname));    
			pmyHeaderCtrl-> GetItem(iCol,   &hdi); //获取表头每列的信息   
			range.SetValue2(COleVariant(hdi.pszText));  //设置每列的内容   
			
			int   nWidth   =   m_ctlList.GetColumnWidth(iCol)/6;   
			//得到第iCol+1列      
			range.AttachDispatch(range.GetItem(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);     
			//设置列宽     
			range.SetColumnWidth(_variant_t((long)nWidth));  
		}   
		
		range   =   sheet.GetRange(COleVariant( _T("A1 ")),   COleVariant(colname));   
		range.SetRowHeight(_variant_t((long)50));//设置行的高度    
		range.SetVerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108    
		
		COleSafeArray   saRet; //COleSafeArray类是用于处理任意类型和维数的数组的类   
		DWORD   numElements[]={m_rows,m_cols};       //行列写入数组   
		saRet.Create(VT_BSTR,   2,   numElements); //创建所需的数组   
		
		range = sheet.GetRange(COleVariant( _T("A2 ")),covOptional); //从A2开始   
		range = range.GetResize(COleVariant((short)m_rows),COleVariant((short)m_cols)); //表的区域   
		
		int t = 1;
		for(   iRow   =   1;   iRow   <=   m_rows;   iRow++)//将列表内容写入EXCEL    
		{
			// m_stStatus.Format("★正在导出数据[%d%%]...",iRow*100/t);
			// UpdateData(FALSE);
			if(GetDate(m_arrTasks[iRow-1].tiStartSet) == stDate)
			{
				if(!SetTaskToExcel(iRow, t, saRet))
				{
					m_stStatus.Format("导出第[%d]行数据失败！",iRow);
					UpdateData(FALSE);
					return false;
				}
				t++;
			}
		}
		range.SetValue2(COleVariant(saRet)); //将得到的数据的saRet数组值放入表格 
	}
  
	book.SaveCopyAs(COleVariant(cStrFile)); //保存到cStrFile文件   
	book.SetSaved(true);  
	books.Close();
	//   
	m_stStatus.Format("导出所有数据到文件 [%s] 成功！",cStrFile);
	UpdateData(FALSE);
	book.ReleaseDispatch();  
	books.ReleaseDispatch();   
  
	app.ReleaseDispatch();  
	app.Quit();
	return true;
}

void CMyDlg::OnDblclkListTaskShow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i;
	bool bFound = false;
	int count = 0;
	int first = -1;
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			if(!bFound)
				first = i;
			bFound = true;
			count ++;
		}
	}
		
	if (!bFound)
	{
		m_stStatus = "★没有选中要查看的数据";
		UpdateData(FALSE);
		return;
	}

	CMsgEditDlg dlg(m_arrTasks[m_arrIndexMap.GetAt(first)], MDT_SHOW, this);
	if (dlg.DoModal() == IDOK)
	{
	}
	else
	{
	}
	*pResult = 0;
}

LRESULT CMyDlg::OnMsgShow( WPARAM wParam,LPARAM lParam )
{
	OnMyShowWindow();
	this->SetFocus();
	return 1L;
}

void CMyDlg::ModifyTime()
{
	UpdateData();
	CTime ti=CTime::GetCurrentTime();
	m_stCurTime = ti.Format("%H:%M:%S");
	UpdateData(FALSE);
}

void CMyDlg::OnStaticMyIcon() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CMyDlg::OnMenuCopyTask()
{
	int i;
	bool bFound = false;
	int count = 0;
	int first = -1;
	for(i=0; i<m_ctlList.GetItemCount(); i++)
	{
		if(m_ctlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlList.GetCheck(i))
		{
			bFound = true;
			count ++;
			m_arrCpy.Add(m_arrTasks[m_arrIndexMap[i]]);
		}
	}
		
	if (!bFound)
	{
		m_stStatus = "★没有选中要复制的任务数据";
		UpdateData(FALSE);
		return;
	}
	
	m_stStatus.Format("成功复制[%d]条数据！",count);
	UpdateData(FALSE);
	return;
}

void CMyDlg::OnMenuClearCopy()
{
	int count = m_arrCpy.GetSize();
	if (count <= 0)
	{
		m_stStatus = "★剪贴板中没有数据！";
		UpdateData(FALSE);
		return;
	}
	m_arrCpy.RemoveAll();
	m_stStatus.Format("成功清空剪贴板数据（条数[%d]）！",count);
	UpdateData(FALSE);
}

void CMyDlg::OnMenuPasteTask()
{
	TaskMsg* ptask;
	if (m_arrCpy.GetSize()<=0)
	{
		m_stStatus = "★剪贴板没有数据！";
		UpdateData(FALSE);
		return;
	}
	for(int i=0; i< m_arrCpy.GetSize(); i++)
	{
		ptask = &(m_arrCpy[i]);
		PrePasteTask(m_arrCpy[i]);
		m_arrTasks.Add(m_arrCpy[i]);
		RedrawListCtrl(m_ctlList.GetItemCount());
		m_nSelectDate = m_ctlSelectDate.GetCurSel();
		RebuildDate();
		m_ctlSelectDate.SetCurSel(m_nSelectDate);
		if (IfRebuildTimeShaft(m_nSelectDate) || m_ctlSelectDate.GetCount() == DEFAULT_SELECT_DATE+1)
			RebuildTimeShaft();
		else
			ClearTimeShaft();
		SetChange(true);
		m_stStatus.Format("粘贴任务[%s]成功！",ptask->stTitle);
		UpdateData(FALSE);
	}
	m_arrCpy.RemoveAll();
}

bool CMyDlg::CheckShow( const TaskMsg& task, const CString stDate )
{
	if(stDate == "所有")
	{
		return true;
	}
	else if(stDate == "今日")
	{
		CString stTmp = GetDate(time(NULL));
		if(stTmp == GetDate(task.tiStartSet))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
    else if(stDate == "未完成")
    {
        CString st;
        //st.Format("Title[%s] Percent=[%d]", task.stTitle, task.nPercent);
        //::MessageBox(st, "未完成检查");
        if(task.nPercent<100)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
	else // 日期
	{
		if(stDate == GetDate(task.tiStartSet))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

void CMyDlg::OnMainDfTasks() 
{
	// TODO: Add your command handler code here
	CMyTaskMgrDlg dlg(this);
	dlg.DoModal();
}

LRESULT CMyDlg::OnTaskBarCreated( WPARAM wParam, LPARAM lParam )
{
	::Shell_NotifyIcon(NIM_ADD,&m_tnid);		//添加托盘图标
	return 0L;
}

void CMyDlg::DoRedrawTrayIcon( bool bReset/*=false*/ )
{
	static bool bIsFirst = true;
	int i=CPubData::setMsg.GetTrayTipType();
	if(CPubData::setMsg.GetTrayTipType()==0)
	{
		if(bIsFirst)
		{
			bIsFirst = false;
			m_tnid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
			m_tnid.uTimeout = 2000;
			m_tnid.dwInfoFlags = NIIF_INFO;
            m_tnid.uVersion = NOTIFYICON_VERSION;
            m_tnid.dwState = NIS_SHAREDICON;
            m_tnid.cbSize=sizeof(NOTIFYICONDATA);
            m_tnid.hWnd= this->GetSafeHwnd();
			if(m_nTipIndex == -1)
			{
				strcpy(m_tnid.szInfo,"未知提醒");
				strcpy(m_tnid.szInfoTitle, "提醒");
			}
			else
			{
				strcpy(m_tnid.szInfo,m_arrTasks[m_nTipIndex].stTitle);
				if(m_nTipType >= 0 && m_nTipType < TT_COUNT)
				{
					strcpy(m_tnid.szInfoTitle, TipTitles[m_nTipType]);
				}
				else
				{
                    strcpy(m_tnid.szInfoTitle, "提醒");
				}
			}
            ::Shell_NotifyIcon(NIM_SETVERSION,&m_tnid);
            if (!::Shell_NotifyIcon(NIM_MODIFY,&m_tnid))		//修改托盘图标
            {
                ::MessageBox("Shell_NotifyIcon ERROR!", "失败", MB_ICONERROR);
            }
		}
		if(bReset)
		{
			bIsFirst = true;
		}
	}
	else
	{
		if(bReset)
		{
			m_nIconIndex = 0;
		}
		else
		{
			m_nIconIndex = !m_nIconIndex;
		}
		m_tnid.hIcon = IconID[m_nIconIndex];
		if (!::Shell_NotifyIcon(NIM_MODIFY,&m_tnid))		//修改托盘图标
        {
            ::MessageBox("Shell_NotifyIcon ERROR!", "失败", MB_ICONERROR);
        }
	}
	//bIsFirst = false;
	//m_tnid.uID=IconID[m_nIconIndex];						//图标资源标号
	
	//::Shell_NotifyIcon(NIM_ADD,&m_tnid);		//修改托盘图标
}

LRESULT CMyDlg::OnTrayIconTipOff( WPARAM wParam,LPARAM lParam )
{
    ::MessageBox("2");
	m_tnid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_tnid.uTimeout = 0;
	m_tnid.dwInfoFlags = 0;
	::Shell_NotifyIcon(NIM_MODIFY, &m_tnid);
	return 0L;
}

void CMyDlg::PrePasteTask( TaskMsg& tsk )
{
	tsk.nIsTipStarted = 0;
	tsk.nIsTipEnded = 0;
	tsk.nIsTipExtra = 1;
	tsk.tiStartSet = time(NULL) + 5 * 60;
	tsk.tipNext.tiEndSet = time(NULL) + 5 * 60 + 10;
	tsk.tiEnd = 0;
	tsk.tiExtraRmd = 0;
    tsk.nPercent = 0;
	strcat(tsk.stDetails, "COPY\n");
}

void CMyDlg::OnExportCurTxt()
{
	UpdateData(TRUE);

    CString stFileName;
    if (!GetExportFileName(FT_XML, stFileName, m_stStatus, this))
    {
        UpdateData(FALSE);
        return;
    }

    TiXmlDocument doc(stFileName);
    TiXmlDeclaration head("1.0", "gb2312", "yes");
    TiXmlComment cmt("我的任务输出文档");
    TiXmlComment cmt2(stFileName);
    TiXmlComment cmtDataFileName(m_stCurFileName);
    TiXmlHandle docHandle( &doc );
    doc.InsertEndChild(head);
    doc.InsertEndChild(cmt);
    doc.InsertEndChild(cmt2);
    doc.InsertEndChild(cmtDataFileName);

    CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   

    int   ncols   = pmyHeaderCtrl-> GetItemCount(); //获取列数   
    int   nrows = m_ctlList.GetItemCount();  //获取行数
    CString stBuf;
    TiXmlElement* pTopEles = NULL; // 一个指向Element的指针
    TiXmlElement* pElmTask = NULL; // 一个指向Element的指针
    TiXmlElement* pElmField = NULL; // 一个指向Element的指针
    TiXmlText* pText = NULL; // 一个指向Text的指针

    pTopEles = new TiXmlElement("Tasks");
    pTopEles->SetAttribute("type", "当前界面");
    pTopEles->SetAttribute("num", nrows);

     for( int  iRow   =   1;   iRow   <=   nrows;   iRow++)//将列表内容写入EXCEL    
     {
    	m_stStatus.Format("★正在导出数据[%d%%]...",iRow*100/nrows);
    	UpdateData(FALSE);
        pElmTask = new TiXmlElement("Task");
        pElmTask->SetAttribute("type", "normal");
        pElmTask->SetAttribute("index", iRow);

    	for   (int iCol = 1;   iCol < ncols;   iCol++)    
    	{
            pElmField = new TiXmlElement(CPubData::gstListTitles[iCol]);
    		CString   szTemp;
    		szTemp=m_ctlList.GetItemText(iRow-1,iCol); //取得m_ctlList控件中的内容
            szTemp.Trim();
            pText = new TiXmlText(LPCTSTR(szTemp));
            pElmField->InsertEndChild(*pText);
            pElmTask->InsertEndChild(*pElmField);
            SAFE_DELETE(pText);
            SAFE_DELETE(pElmField);
    	}
        pTopEles->InsertEndChild(*pElmTask);
        SAFE_DELETE(pElmTask);
     }
    doc.InsertEndChild(*pTopEles);
    SAFE_DELETE(pTopEles);
    doc.SaveFile();
    m_stStatus.Format("导出[%d]条数据到[%s]成功！", nrows, LPCTSTR(stFileName));
    UpdateData(FALSE);
}

void CMyDlg::OnExportAllTxt()
{
    UpdateData(TRUE);
    CString stFileName;
    if (!GetExportFileName(FT_XML, stFileName, m_stStatus, this))
    {
        //::MessageBox(_T("选择文件失败！"));
        UpdateData(FALSE);
        return;
    }

    TiXmlDocument doc(stFileName);
    TiXmlDeclaration head("1.0", "gb2312", "yes");
    TiXmlComment cmt("我的任务输出文档");
    TiXmlComment cmt2(stFileName);
    TiXmlComment cmtDataFileName(m_stCurFileName);
    TiXmlHandle docHandle( &doc );
    doc.InsertEndChild(head);
    doc.InsertEndChild(cmt);
    doc.InsertEndChild(cmt2);
    doc.InsertEndChild(cmtDataFileName);

    CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   

    int   ncols   = pmyHeaderCtrl->GetItemCount(); //获取列数   
    int   nrows = m_arrTasks.GetCount();  // 数量
    CString stBuf;
    TiXmlElement* pTopEles = NULL; // 一个指向Element的指针
    TiXmlElement* pElmTask = NULL; // 一个指向Element的指针
    TiXmlElement* pElmField = NULL; // 一个指向Element的指针
    TiXmlText* pText = NULL; // 一个指向Text的指针

    pTopEles = new TiXmlElement("Tasks");
    pTopEles->SetAttribute("type", "所有任务");
    pTopEles->SetAttribute("num", nrows);

    CString   szTemp;

    for( int  iRow   =   0;   iRow   <  nrows;   iRow++)//将列表内容写入EXCEL    
    {
        m_stStatus.Format("★正在导出数据[%d%%]...",(iRow+1)*100/nrows);
        UpdateData(FALSE);
        pElmTask = new TiXmlElement("Task");
        pElmTask->SetAttribute("type", "normal");
        pElmTask->SetAttribute("index", iRow);

        for   (int iCol = 1;   iCol < ncols + 1;   iCol++)    
        {
            pElmField = new TiXmlElement(CPubData::gstListTitles[iCol]);
            GetNormalTaskDataByCol(szTemp, m_arrTasks[iRow], iCol);
            pText = new TiXmlText(LPCTSTR(szTemp));
            pElmField->InsertEndChild(*pText);
            pElmTask->InsertEndChild(*pElmField);
            SAFE_DELETE(pText);
            SAFE_DELETE(pElmField);
        }
        pTopEles->InsertEndChild(*pElmTask);
        SAFE_DELETE(pElmTask);
    }
    doc.InsertEndChild(*pTopEles);
    SAFE_DELETE(pTopEles);
    doc.SaveFile();

    m_stStatus.Format("导出[%d]条数据到[%s]成功！", nrows, LPCTSTR(stFileName));
    UpdateData(FALSE);
}

void CMyDlg::OnExportAllByDayTxt()
{
    UpdateData(TRUE);
    CString stFileName;
    if (!GetExportFileName(FT_XML, stFileName, m_stStatus, this))
    {
        //::MessageBox(_T("选择文件失败！"));
        UpdateData(FALSE);
        return;
    }

    TiXmlDocument doc(stFileName);
    TiXmlDeclaration head("1.0", "gb2312", "yes");
    TiXmlComment cmt("我的任务输出文档");
    TiXmlComment cmt2(stFileName);
    TiXmlComment cmtDataFileName(m_stCurFileName);
    TiXmlHandle docHandle( &doc );
    doc.InsertEndChild(head);
    doc.InsertEndChild(cmt);
    doc.InsertEndChild(cmt2);
    doc.InsertEndChild(cmtDataFileName);

    CHeaderCtrl   *pmyHeaderCtrl= m_ctlList.GetHeaderCtrl(); //获取表头   

    int   ncols  = pmyHeaderCtrl->GetItemCount(); //获取列数   
    int   nrows  = m_arrTasks.GetCount();  // 数量
    int   nCount = 0; // 真正数量
    CString stBuf;
    CString stDate;
    TiXmlElement* pTopEles = NULL; // 一个指向Element的指针
    TiXmlElement* pElmTask = NULL; // 一个指向Element的指针
    TiXmlElement* pElmField = NULL; // 一个指向Element的指针
    TiXmlText* pText = NULL; // 一个指向Text的指针

    for(int index = DEFAULT_SELECT_DATE; index < m_ctlSelectDate.GetCount(); index++)
    {
        pTopEles = new TiXmlElement("Tasks");
        m_ctlSelectDate.GetLBText(index, stDate);
        pTopEles->SetAttribute("type", stDate);

        CString   szTemp;
        nCount = 0;
        for( int  iRow   =   0;   iRow   <   nrows;   iRow++)//将列表内容写入EXCEL    
        {
            m_stStatus.Format("★正在导出数据[%d%%]...",(iRow+1)*100/nrows);
            UpdateData(FALSE);
            if(GetDate(m_arrTasks[iRow].tiStartSet) != stDate)
            {
                continue;
            }
            ++ nCount;
            pElmTask = new TiXmlElement("Task");
            pElmTask->SetAttribute("type", "normal");
            pElmTask->SetAttribute("index", nCount);

            for   (int iCol = 1;   iCol < ncols + 1;   iCol++)    
            {
                pElmField = new TiXmlElement(CPubData::gstListTitles[iCol]);
                GetNormalTaskDataByCol(szTemp, m_arrTasks[iRow], iCol);
                pText = new TiXmlText(LPCTSTR(szTemp));
                pElmField->InsertEndChild(*pText);
                pElmTask->InsertEndChild(*pElmField);
                SAFE_DELETE(pText);
                SAFE_DELETE(pElmField);
            }
            pTopEles->InsertEndChild(*pElmTask);
            SAFE_DELETE(pElmTask);
        }
        pTopEles->SetAttribute("num", nCount);
        doc.InsertEndChild(*pTopEles);
        SAFE_DELETE(pTopEles);
    }
    doc.SaveFile();

    m_stStatus.Format("导出[%d]条数据到[%s]成功！", nrows, LPCTSTR(stFileName));
    UpdateData(FALSE);
}

void CMyDlg::CheckDfTaskRemind()
{
    //ADD_NORMAL("CheckDfTaskRemind Start!");
    static bool gbRunning = false;
    if (gbRunning) return;
    gbRunning = true;
    int i=0;
    time_t tiNow = time(NULL);
    int mins = 0;
    int type;
    for(i=0; i<CPubData::gArrDfTasks.GetCount(); i++)
    {
        // 计算提醒时间
        TaskMsg& tsk = CPubData::gArrDfTasks[i];
        
        if (CheckMsgTypeTiming(tsk.nMsgType))
        {
            m_nTipType = TT_CO_FIXTIME;
        }
        else if(CheckMsgTypeRepeat(tsk.nMsgType))
        {
            m_nTipType = TT_CO_CIRCUL;
        }
        else
        {
            continue;
        }

        if (tsk.tiNextRmd == 0)
        {
            tsk.CountNextRmdTime(true);
            ADD_NORMAL("默认任务[%s]第一次计算执行时间[%s]", tsk.stTitle, LPCTSTR(GetDateTime(tsk.tiNextRmd)));
            CPubData::gArrDfTasks.SetChange();
        }
        mins = 0;
        if (tsk.tiNextRmd <= tiNow)
        {
            if (tsk.tipNext.rt.nTypeTime == RMD_TT_EXACT_TIME
                && tsk.nIsTipEnded == 1)
            {
                continue;
            }
            m_nTipIndex = i;
            bool bRet = false;
            if (CheckMsgTypeRemind(tsk.nMsgType) && !CheckMsgTypeRunCmd(tsk.nMsgType))
            {
                if (m_bIsOnStart) // 开始时批量处理
                {
                    m_arrTimeoutTasks.Add(&(tsk));
                }
                else
                {
                    ADD_DEBUG("Start DoRemind[%s][%s]", tsk.stTitle, tsk.stDetails);
                    bRet = DoRemind(tsk, type, mins);
                    if (!bRet)
                    {
                        continue;
                    }
                    ADD_DEBUG("End DoRemind");
                }
            }
            if (CheckMsgTypeRunCmd(tsk.nMsgType))
            {
                if (m_bIsOnStart) // 开始时批量处理
                {
                    m_arrTimeoutTasks.Add(&(tsk));
                }
                else
                {
                    ADD_DEBUG("Start DoTaskCommand[%s][%s]", tsk.stTitle, tsk.stCmd);
                    bRet = DoTaskCommand(tsk);
                    if(!bRet)
                    {
                        continue;
                    }
                    ADD_DEBUG("End DoTaskCommand");
                }
            }

            // 计算下次执行时间
            {
                //ADD_DEBUG("Before CountNextRmdTime");
                tsk.CountNextRmdTime(false);
                ADD_NORMAL("默认任务[%s]重算执行时间[%s]", tsk.stTitle, LPCTSTR(GetDateTime(tsk.tiNextRmd)));
                //ADD_DEBUG("After CountNextRmdTime");
                if (tsk.tipNext.rt.nTypeTime == RMD_TT_EXACT_TIME)
                {
                    tsk.nIsTipEnded = 1;
                }
                CPubData::gArrDfTasks.SetChange();
                //ADD_DEBUG("After SetChange");
            }
        }
    }
    gbRunning = false;
}

bool CMyDlg::SortBySubItemAndRedraw( int iSubItem )
{
    SetColumnSortFlag(0, SF_DESC_FLAG);
    SetColumnSortFlag(m_iSortIndex, SF_NO_FLAG);
    if (m_iSortIndex == iSubItem)
    {
        m_bIsAscending = !m_bIsAscending;
    }
    else
    {
        m_bIsAscending = true;
    }
    m_iSortIndex = iSubItem;
    SetColumnSortFlag(m_iSortIndex, 
        m_bIsAscending?SF_ASC_FLAG:SF_DESC_FLAG);
    bool bRefresh=false;

    switch(m_iSortIndex)
    {
    case 1:
        if (m_bIsAscending)
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByTitleAsc);
        }
        else
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByTitleDesc);
        }
        bRefresh = true;
        break;
    case 2:
        if (m_bIsAscending)
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByStartTimeAsc);
        }
        else
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByStartTimeDesc);
        }
        bRefresh = true;
        break;
    case 3:
        if (m_bIsAscending)
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByEndTimeAsc);
        }
        else
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByEndTimeDesc);
        }
        bRefresh = true;
        break;
    case 4:
        if (m_bIsAscending)
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByPercentAsc);
        }
        else
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByPercentDesc);
        }
        bRefresh = true;
        break;
    case 5:
        if (m_bIsAscending)
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByFinishTimeAsc);
        }
        else
        {
            qsort(m_arrTasks.GetData(), m_arrTasks.GetSize(), sizeof(TaskMsg), &CmpTskByFinishTimeDesc);
        }
        bRefresh = true;
        break;
    default:
        //::MessageBox("Not Support!");
        SetColumnSortFlag(m_iSortIndex, SF_NO_FLAG);
        m_iSortIndex = -1;
        break;
    }

    if (bRefresh)
    {
        return RedrawListCtrl();
    }
    else
    {
        return true;
    }
    
}

void CMyDlg::SetColumnSortFlag( int iSortIndex, TagSortFlag flag )
{
    if (iSortIndex < 0)
    {
        return;
    }
    LVCOLUMN column;
    CString st;
    char stTmp[512] = {0};
    int i=0;

    column.iSubItem = iSortIndex;
    column.mask = LVCF_TEXT;
    
    if (iSortIndex == 0)
    {
        if (SF_DESC_FLAG == flag)
        {
            column.pszText = "全选";
        }
        else
        {
            column.pszText = "全不选";
        }
    }
    else
    {
        st = CPubData::gstListTitles[iSortIndex];
        if (SF_ASC_FLAG == flag)
        {
            st += "[↑]";
        }
        else if(SF_DESC_FLAG == flag)
        {
            st += "[↓]";
        }
        memccpy(stTmp, LPCTSTR(st), '\0', sizeof(stTmp));
        stTmp[sizeof(stTmp)-1] = '\0';
        column.pszText = stTmp;
    }
    m_ctlList.SetColumn(iSortIndex, &column);
}

void CMyDlg::OnAbout()
{
    // TODO: 在此添加命令处理程序代码
    OnStaticMyIcon();
}

void CMyDlg::WinHelp( DWORD_PTR dwData, UINT nCmd /*= HELP_CONTEXT */ )
{
    OnStaticMyIcon();
}

void CMyDlg::OnStart()
{
    ADD_TRACE("开始批量处理 ...\n");
    m_bIsOnStart = true;

    // 采集过期任务
    CheckTaskRemind();
    CheckDfTaskRemind();

    // 处理过期任务
    if (GetTimeoutTaskCount() > 0)
    {
        CTimeoutTaskDlg dlg(this);
        dlg.DoModal();
        m_arrTimeoutTasks.RemoveAll();
    }

    m_bIsOnStart = false;
    ADD_TRACE("结束批量处理 ...\n");
}

bool CMyDlg::DoTaskCommandImpl()
{
    int taskIndex = m_nTipIndex;
    int type = 0, mins = 0; // 暂时无作用
    if(!DoRemind(m_arrTasks[taskIndex], type, mins))
    {
        m_arrTasks[taskIndex].nIsTipStarted = 0;
        return false;
    }
    if (mins > 0)
    {
        time_t tiNow2 = time(NULL);
        m_arrTasks[taskIndex].nIsTipExtra = 0;
        m_arrTasks[taskIndex].tiExtraRmd = tiNow2 + mins*60;
        m_stStatus.Format("任务[%s]将会在%s(%d分钟后)再次提醒！",
            m_arrTasks[taskIndex].stTitle, LPCTSTR(GetDateTime(m_arrTasks[taskIndex].tiExtraRmd)), mins);
        UpdateData(FALSE);
    }
    SetChange(true);
    for(int j=0; j<m_arrIndexMap.GetSize(); j++)
    {
        if (m_arrIndexMap[j]==taskIndex)
        {
            RedrawListCtrl(j);
            break;
        }
    }
    if((IfRebuildTimeShaft(m_nSelectDate) 
          || m_ctlSelectDate.GetCount()==DEFAULT_SELECT_DATE+1) 
        && m_ctlList.GetItemCount()>0)
        RebuildTimeShaft();
    else
        ClearTimeShaft();

    return true;
}

void CMyDlg::OnMenuMainIniEdit()
{
    // TODO: 在此添加命令处理程序代码
    CString stLogFile = CPubData::GetCurrentDir() + "\\task.ini";
    if (m_stOpenLogCmd.IsEmpty())
    {
        ::ShellExecute(NULL,"open","notepad.exe",LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_SHOWDEFAULT);
    }
    else
    {
        int i = m_stOpenLogCmd.Find(".bat");
        int j = m_stOpenLogCmd.Find(".cmd");
        if ( (i>0 && i+4 == m_stOpenLogCmd.GetLength()) 
            || (j>0 && j+4 == m_stOpenLogCmd.GetLength())  )
        {
            ::ShellExecute(NULL,"open", LPCTSTR(m_stOpenLogCmd),LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_HIDE);
        }
        else
        {
            ::ShellExecute(NULL,"open", LPCTSTR(m_stOpenLogCmd),LPCTSTR(stLogFile),LPCTSTR(stLogFile),SW_SHOWDEFAULT);
        }
    }
}
