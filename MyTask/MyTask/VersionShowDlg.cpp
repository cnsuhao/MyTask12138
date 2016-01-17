// VersionShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "VersionShowDlg.h"
#include "PubData.h"


// CVersionShowDlg 对话框

IMPLEMENT_DYNAMIC(CVersionShowDlg, CDialog)

CVersionShowDlg::CVersionShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVersionShowDlg::IDD, pParent)
    , m_stVerList(_T(""))
{

}

CVersionShowDlg::~CVersionShowDlg()
{
}

void CVersionShowDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_VER_LIST, m_ctrlVerList);
    DDX_CBString(pDX, IDC_COMBO_VER_LIST, m_stVerList);
    DDX_Control(pDX, IDC_LIST_VERSION, m_ctrlDetailsList);
    DDX_Control(pDX, IDOK, m_btnExit);
}


BEGIN_MESSAGE_MAP(CVersionShowDlg, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_VER_LIST, &CVersionShowDlg::OnCbnSelchangeComboVerList)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CVersionShowDlg::SetVersionInfo( const char* st[], int size_ )
{
    std::string stTitle;
    std::string stCur;
    VecString* plist = new VecString();
    int i=0;
    bool bFirst = true;
    vecTitle.push_back("all");
    while (i++<size_)
    {
        if (bFirst)
        {
            stTitle = st[i-1];
            bFirst = false;
            continue;
        }
        else
        {
            stCur = st[i-1];
            if (stCur.size() == 0)
            {
                bFirst = true;
                vecTitle.push_back(stTitle);
                vecVers.push_back(plist);
                plist = new VecString();
                continue;
            }
            else
            {
                plist->push_back(stCur);
            }
        }
    }
}

BOOL CVersionShowDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.
    m_btnExit.LoadBitmaps(IDB_BITMAP_BTN_GEN, IDB_BITMAP_BTN_SEL,
        IDB_BITMAP_BTN_FOCUS, IDB_BITMAP_BTN_GRAY);

    SetVersionInfo(CPubData::gstVersionModifyString, 
        CPubData::gnVersionModifySize);
    for(int i=0; i<(int)vecTitle.size(); ++i)
    {
        m_ctrlVerList.AddString(vecTitle[i].c_str());
    }

    m_ctrlVerList.SetCurSel(0);
    DoSelectVersion(0);
    return UpdateData(FALSE);
}

void CVersionShowDlg::DoSelectVersion( int index )
{
    while(m_ctrlDetailsList.GetCount() > 0)
        m_ctrlDetailsList.DeleteString(0);
    if (index <= 0)
    {
        for (int i=0; i<(int)vecVers.size(); ++i)
        {
            m_ctrlDetailsList.AddString(vecTitle[i+1].c_str());

            VecString* pVec = vecVers[i];
            for(VecString::const_iterator it =pVec->begin();
                it != pVec->end(); ++it)
            {
                m_ctrlDetailsList.AddString(it->c_str());
            }
        }
        return;
    }

    m_ctrlDetailsList.AddString(vecTitle[index].c_str());
    VecString* pVec = vecVers[index-1];
    for(VecString::const_iterator it =pVec->begin();
        it != pVec->end(); ++it)
    {
        m_ctrlDetailsList.AddString(it->c_str());
    }
}


// CVersionShowDlg 消息处理程序

void CVersionShowDlg::OnCbnSelchangeComboVerList()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    DoSelectVersion(m_ctrlVerList.GetCurSel());
    UpdateData(FALSE);
}

HBRUSH CVersionShowDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
