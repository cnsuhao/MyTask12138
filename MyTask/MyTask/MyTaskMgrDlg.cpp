// MyTaskMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyTaskMgrDlg.h"
#include "PubData.h"
#include "MyDfTaskEditDlg.h"
//#include "�ҵ�����Dlg.h"
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
        ADD_NORMAL("���Ĭ������ [%s] �ɹ���",dlg.GetTaskPointer()->stTitle);
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

	// ��ʼ�����
	DWORD dwStyle = m_ctlListAll.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	dwStyle |= LVS_OWNERDRAWFIXED;
	m_ctlListAll.SetExtendedStyle(dwStyle); //������չ���
	m_ctlListAll.SetBkColor(CLR_NONE);
    m_ctlListAll.SetTextBkColor(CLR_NONE);
	
	m_ctlListAll.InsertColumn( 0, "����", LVCFMT_LEFT, 50 );//������
	m_ctlListAll.InsertColumn( 1, "����", LVCFMT_LEFT, 150 );//������
	m_ctlListAll.InsertColumn( 2, "�´�����ʱ��", LVCFMT_CENTER, 140 );
	m_ctlListAll.InsertColumn( 3, "����", LVCFMT_LEFT, 180 );
	m_ctlListAll.InsertColumn( 4, "��ϸ��Ϣ", LVCFMT_LEFT, 260 );

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
	CString stTitles="�༭Ĭ������ ";
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
		::MessageBox("��û��ѡ��Ҫ�༭�����ݣ�","δѡ������",3);
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
				stTitles += " �ɹ���";
				::MessageBox(stTitles, "�༭�ɹ�",3);
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
	CString stTitles="ɾ��Ĭ������ ";
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
		::MessageBox("��û��ѡ��Ҫɾ�������ݣ�","δѡ������", 3);
		return;
	}
	if (::MessageBox("ȷ��ɾ��������ѡ������","ȷ��ɾ��", 10, FALSE) == IDOK)
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
		stTitles += " �ɹ���";
		::MessageBox(stTitles, "ɾ���ɹ�", 3);
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
		nRow = m_ctlListAll.InsertItem(m_ctlListAll.GetItemCount(), LPCTSTR(st));// ������
	}
	else
	{
		m_ctlListAll.SetItemText(nRow, 0, LPCTSTR(st));// ��������
	}
	
	m_ctlListAll.SetItemText(nRow, 1, tsk.stTitle);// ��������
		
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
        st = "[����]";
    }
    if (CheckMsgTypeRunCmd(tsk.nMsgType))
    {
        st += CString("[")+tsk.stCmd+"]";
    }
    
    if (st.IsEmpty())
    {
        st = "[δ����]";
    }
	m_ctlListAll.SetItemText(nRow, 3, LPCTSTR(st));
	m_ctlListAll.SetItemText(nRow, 4, tsk.stDetails);
	return;
}


void CMyTaskMgrDlg::OnBnClickedEditDfExport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
    TiXmlComment cmt("�ҵ���������ĵ�");
    TiXmlComment cmt2(stFileName);
    TiXmlComment cmtDataFileName(CPubData::GetDataFileName(FILE_NAME_DF_TASK));
    TiXmlHandle docHandle( &doc );
    doc.InsertEndChild(head);
    doc.InsertEndChild(cmt);
    doc.InsertEndChild(cmt2);
    doc.InsertEndChild(cmtDataFileName);

    int   ncols   = 8; //��ȡ����   
    int   nrows = CPubData::gArrDfTasks.GetCount();  // ����
    CString stBuf;
    TiXmlElement* pTopEles = NULL; // һ��ָ��Element��ָ��
    TiXmlElement* pElmTask = NULL; // һ��ָ��Element��ָ��
    TiXmlElement* pElmField = NULL; // һ��ָ��Element��ָ��
    TiXmlText* pText = NULL; // һ��ָ��Text��ָ��

    pTopEles = new TiXmlElement("Tasks");
    pTopEles->SetAttribute("type", "��������");
    pTopEles->SetAttribute("num", nrows);

    CString   szTemp;

    for( int  iRow   =   0;   iRow   <  nrows;   iRow++)//���б�����д��EXCEL    
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

    stErrMsg.Format("����[%d]�����ݵ�[%s]�ɹ���", nrows, LPCTSTR(stFileName));
    ::MessageBox(stErrMsg, "�����ɹ�", 5, TRUE);
}

void CMyTaskMgrDlg::OnBnClickedEditDfImport()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    ::MessageBox("Not Support");
}
