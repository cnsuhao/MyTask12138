// TimeoutTaskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "TimeoutTaskDlg.h"
#include "PubData.h"
#include "�ҵ�����Dlg.h"


// CTimeoutTaskDlg �Ի���

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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_ITimeoutTaskDlg ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

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


// CTimeoutTaskDlg ��Ϣ�������

HBRUSH CTimeoutTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����

    {
        pDC->SetBkMode(TRANSPARENT);
        return CPubData::gBr;
    }

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}

void CTimeoutTaskDlg::OnBnClickedOkKnown()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    OnOK();
}
