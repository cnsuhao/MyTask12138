// TimeoutTaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "TimeoutTaskDlg.h"
#include "PubData.h"
#include "我的任务Dlg.h"


// CTimeoutTaskDlg 对话框

IMPLEMENT_DYNAMIC(CTimeoutTaskDlg, CDialog)

CTimeoutTaskDlg::CTimeoutTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeoutTaskDlg::IDD, pParent)
{

	EnableAutomation();

}

CTimeoutTaskDlg::~CTimeoutTaskDlg()
{
}

void CTimeoutTaskDlg::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CDialog::OnFinalRelease();
}

void CTimeoutTaskDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TIMEOUT_TASK, m_listTasks);
    DDX_Control(pDX, ID_OK_KNOWN, m_btnAllKnown);
}


BEGIN_MESSAGE_MAP(CTimeoutTaskDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(ID_OK_KNOWN, &CTimeoutTaskDlg::OnBnClickedOkKnown)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTimeoutTaskDlg, CDialog)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_ITimeoutTaskDlg 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {E18F866F-769B-43BF-B22A-922CD025800F}
static const IID IID_ITimeoutTaskDlg =
{ 0xE18F866F, 0x769B, 0x43BF, { 0xB2, 0x2A, 0x92, 0x2C, 0xD0, 0x25, 0x80, 0xF } };

BEGIN_INTERFACE_MAP(CTimeoutTaskDlg, CDialog)
	INTERFACE_PART(CTimeoutTaskDlg, IID_ITimeoutTaskDlg, Dispatch)
END_INTERFACE_MAP()

BOOL CTimeoutTaskDlg::OnInitDialog()
{
    if (!CDialog::OnInitDialog())
    {
        return FALSE;
    }

    m_btnAllKnown.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL, IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

    CMyDlg * pDlg = (CMyDlg *)GetParent();
    int size = pDlg->GetTimeoutTaskCount();
    {
        CString st;
        for(int i=0; i<size; ++i)
        {
            PTaskMsg pTsk = pDlg->GetTimeoutTaskAt(i);
            st.Format("%-32s%s", pTsk->stTitle, pTsk->stDetails);
            m_listTasks.AddString(st);
        }
    }

    UpdateData(FALSE);

    return TRUE;
}


// CTimeoutTaskDlg 消息处理程序

HBRUSH CTimeoutTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何属性

    {
        pDC->SetBkMode(TRANSPARENT);
        return CPubData::gBr;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

void CTimeoutTaskDlg::OnBnClickedOkKnown()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}
