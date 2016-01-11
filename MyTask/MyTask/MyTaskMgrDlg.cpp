// MyTaskMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyTaskMgrDlg.h"
#include "PubData.h"
#include "MyDfTaskEditDlg.h"
//#include "我的任务Dlg.h"
#include "MyLog.h"
#include "tinyxml/tinyxml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTaskMgrDlg dialog


CMyTaskMgrDlg::CMyTaskMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyTaskMgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyTaskMgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyTaskMgrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMyTaskMgrDlg)
    DDX_Control(pDX, IDC_LIST_DF_TASKS, m_ctlListAll);
    DDX_Control(pDX, ID_OK_EDIT_DF, m_btnExit);
    DDX_Control(pDX, ID_EDIT_DF_MODIFY, m_btnMod);
    DDX_Control(pDX, ID_EDIT_DF_DEL, m_btnDel);
    DDX_Control(pDX, ID_EDIT_DF_ADD, m_btnAdd);
    DDX_Control(pDX, ID_EDIT_DF_EXPORT, m_btnExport);
    DDX_Control(pDX, ID_EDIT_DF_IMPORT, m_btnImport);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyTaskMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CMyTaskMgrDlg)
	ON_BN_CLICKED(ID_EDIT_DF_ADD, OnEditDfAdd)
	ON_BN_CLICKED(ID_EDIT_DF_MODIFY, OnEditDfModify)
	ON_BN_CLICKED(ID_EDIT_DF_DEL, OnEditDfDel)
	ON_BN_CLICKED(ID_OK_EDIT_DF, OnOkEditDf)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_EDIT_DF_EXPORT, &CMyTaskMgrDlg::OnBnClickedEditDfExport)
    ON_BN_CLICKED(ID_EDIT_DF_IMPORT, &CMyTaskMgrDlg::OnBnClickedEditDfImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTaskMgrDlg message handlers

void CMyTaskMgrDlg::OnEditDfAdd() 
{
	// TODO: Add your control notification handler code here
	TaskMsg msg;
	CMyDfTaskEditDlg dlg(msg, MDT_NEW, this);
	if(IDOK == dlg.DoModal())
	{
		CPubData::gArrDfTasks.AddTask(*(dlg.GetTaskPointer()));
		AddDfTask(*(dlg.GetTaskPointer()), CPubData::gArrDfTasks.GetCount()-1);
        ADD_NORMAL("添加默认任务 [%s] 成功！",dlg.GetTaskPointer()->stTitle);
	}
}

BOOL CMyTaskMgrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;
	m_btnAdd.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnDel.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnExit.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
	m_btnMod.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
    m_btnExport.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);
    m_btnImport.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

	// 初始化表格
	DWORD dwStyle = m_ctlListAll.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_OWNERDRAWFIXED;
	m_ctlListAll.SetExtendedStyle(dwStyle); //设置扩展风格
	m_ctlListAll.SetBkColor(CLR_NONE);
    m_ctlListAll.SetTextBkColor(CLR_NONE);
	
	m_ctlListAll.InsertColumn( 0, "启用", LVCFMT_LEFT, 50 );//插入列
	m_ctlListAll.InsertColumn( 1, "标题", LVCFMT_LEFT, 150 );//插入列
	m_ctlListAll.InsertColumn( 2, "下次提醒时间", LVCFMT_CENTER, 140 );
	m_ctlListAll.InsertColumn( 3, "操作", LVCFMT_LEFT, 180 );
	m_ctlListAll.InsertColumn( 4, "详细信息", LVCFMT_LEFT, 260 );

	for(i=0; i<CPubData::gArrDfTasks.GetCount(); i++)
	{
		AddDfTask(CPubData::gArrDfTasks[i], i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyTaskMgrDlg::OnEditDfModify() 
{
	// TODO: Add your control notification handler code here
	int i;
	bool bFound = false;
	int count = 0;
	CString stTitles="编辑默认任务 ";
	for(i=0; i<m_ctlListAll.GetItemCount(); i++)
	{
		if(m_ctlListAll.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlListAll.GetCheck(i))
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		::MessageBox("★没有选中要编辑的数据！","未选中任务",3);
		return;
	}
	
	for(i=0; i<m_ctlListAll.GetItemCount(); i++)
	{
		if(m_ctlListAll.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlListAll.GetCheck(i))
		{
			//m_ctlListAll.DeleteItem(i);
			stTitles += "[";
			stTitles += CPubData::gArrDfTasks[i].stTitle;
			stTitles += "]";
			//m_arrTasks.RemoveAt(m_arrIndexMap.GetAt(i));
			CMyDfTaskEditDlg dlg(CPubData::gArrDfTasks[i], MDT_EDIT, this);
			if(IDOK == dlg.DoModal())
			{
                CPubData::gArrDfTasks[i] = *(dlg.GetTaskPointer());
				stTitles += " 成功！";
				::MessageBox(stTitles, "编辑成功",3);
                ADD_NORMAL("%s",stTitles);
				CPubData::gArrDfTasks.SetChange(true);
                SetDataToCtrl(CPubData::gArrDfTasks[i], i, FALSE);
			}
			break;
		}
	}
	
	UpdateData(FALSE);
}

void CMyTaskMgrDlg::OnEditDfDel() 
{
	// TODO: Add your control notification handler code here
	int i;
	bool bFound = false;
	int count = 0;
	CString stTitles="删除默认任务 ";
	for(i=0; i<m_ctlListAll.GetItemCount(); i++)
	{
		if(m_ctlListAll.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlListAll.GetCheck(i))
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		::MessageBox("★没有选中要删除的数据！","未选中任务", 3);
		return;
	}
	if (::MessageBox("确认删除所有所选的任务？","确认删除", 10, FALSE) == IDOK)
	{
		while (1)
		{
			bFound = false;
			for(i=0; i<m_ctlListAll.GetItemCount(); i++)
			{
				if(m_ctlListAll.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_ctlListAll.GetCheck(i))
				{
					m_ctlListAll.DeleteItem(i);
					stTitles += "[";
					stTitles += CPubData::gArrDfTasks[i].stTitle;
					stTitles += "]";
					//m_arrTasks.RemoveAt(m_arrIndexMap.GetAt(i));
					CPubData::gArrDfTasks.DelMsgAt(i);
					bFound = true;
					count ++;
					break;
				}
			}
			
			if (!bFound)
				break;
		}
		stTitles += " 成功！";
		::MessageBox(stTitles, "删除成功", 3);
        ADD_NORMAL("%s",stTitles);
		UpdateData(FALSE);
	}
}

void CMyTaskMgrDlg::OnOkEditDf() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

HBRUSH CMyTaskMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CMyTaskMgrDlg::AddDfTask(const TaskMsg& tsk, int index)
{
	SetDataToCtrl(tsk, index, TRUE);
    CPubData::gArrDfTasks.SetChange(true);
}

void CMyTaskMgrDlg::SetDataToCtrl( const TaskMsg& tsk, int index, bool IsAdd )
{
	CString st;
	CTime tiTmp;
	int i = index;
	st.Format("%-2d", index + 1);
	int nRow = index;
	if (IsAdd)
	{
		nRow = m_ctlListAll.InsertItem(m_ctlListAll.GetItemCount(), LPCTSTR(st));// 插入行
	}
	else
	{
		m_ctlListAll.SetItemText(nRow, 0, LPCTSTR(st));// 设置数据
	}
	
	m_ctlListAll.SetItemText(nRow, 1, tsk.stTitle);// 设置数据
		
	tiTmp = tsk.tiNextRmd;
	st = tiTmp.Format("%Y-%m-%d %H:%M");
	m_ctlListAll.SetItemText(nRow, 2, LPCTSTR(st));
		
// 	tiTmp = tsk.tiEndSet;
// 	st = tiTmp.Format("%Y-%m-%d %H:%M");
// 	m_ctlListAll.SetItemText(nRow, 3, LPCTSTR(st));
		
// 	st.Format("	%d%%",tsk.nPercent);
// 	m_ctlListAll.SetItemText(nRow, 4, LPCTSTR(st));
	
	//tiTmp = tsk.tiEnd;
	//if (tiTmp != 0)
	//{
	//	st = tiTmp.Format("%Y-%m-%d %H:%M");
	//}
	//else
	//{
	//	st.Empty();
	//}
    st.Empty();
    if (CheckMsgTypeRemind(tsk.nMsgType))
    {
        st = "[提醒]";
    }
    if (CheckMsgTypeRunCmd(tsk.nMsgType))
    {
        st += CString("[")+tsk.stCmd+"]";
    }
    
    if (st.IsEmpty())
    {
        st = "[未启用]";
    }
	m_ctlListAll.SetItemText(nRow, 3, LPCTSTR(st));
	m_ctlListAll.SetItemText(nRow, 4, tsk.stDetails);
	return;
}


void CMyTaskMgrDlg::OnBnClickedEditDfExport()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString stFileName;
    CString stErrMsg;
    if (!GetExportFileName(FT_XML, stFileName, stErrMsg, this))
    {
        ::MessageBox(stErrMsg);
        return;
    }

    TiXmlDocument doc(stFileName);
    TiXmlDeclaration head("1.0", "gb2312", "yes");
    TiXmlComment cmt("我的任务输出文档");
    TiXmlComment cmt2(stFileName);
    TiXmlComment cmtDataFileName(CPubData::GetDataFileName(FILE_NAME_DF_TASK));
    TiXmlHandle docHandle( &doc );
    doc.InsertEndChild(head);
    doc.InsertEndChild(cmt);
    doc.InsertEndChild(cmt2);
    doc.InsertEndChild(cmtDataFileName);

    int   ncols   = 8; //获取列数   
    int   nrows = CPubData::gArrDfTasks.GetCount();  // 数量
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
        pElmTask = new TiXmlElement("Task");
        pElmTask->SetAttribute("type", "XXXXX");
        pElmTask->SetAttribute("index", iRow);

        for   (int iCol = 1;   iCol < ncols + 1;   iCol++)    
        {
            pElmField = new TiXmlElement(CPubData::gstDefaultListTitles[iCol]);
            GetDefaultTaskDataByCol(szTemp, CPubData::gArrDfTasks[iRow], iCol);
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

    stErrMsg.Format("导出[%d]条数据到[%s]成功！", nrows, LPCTSTR(stFileName));
    ::MessageBox(stErrMsg, "导出成功", 5, TRUE);
}

void CMyTaskMgrDlg::OnBnClickedEditDfImport()
{
    // TODO: 在此添加控件通知处理程序代码
    ::MessageBox("Not Support");
}
