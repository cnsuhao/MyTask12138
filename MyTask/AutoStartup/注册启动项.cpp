// ע��������.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ע��������.h"
#include "ע��������Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegStartUpApp

BEGIN_MESSAGE_MAP(CRegStartUpApp, CWinApp)
	//{{AFX_MSG_MAP(CRegStartUpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegStartUpApp construction

CRegStartUpApp::CRegStartUpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRegStartUpApp object

CRegStartUpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRegStartUpApp initialization

BOOL CRegStartUpApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//CRegStartUPDlg dlg;
	m_pMainWnd = NULL;
	//int nResponse = dlg.DoModal();

// 	for (int i=0;i<__argc;i++)
// 	{
// 		AfxMessageBox(__argv[i]);
// 	}
	bool isAdd = true;
	if(__argc > 1)
	{
		CString stTmp = __argv[1];
		if(stTmp == "off")
			isAdd = false;
	}

	// �޸�ע���
	{
		CString    sPath;
		GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
		sPath.Replace("ע��������","�ҵ�����");
		sPath.ReleaseBuffer();
		LPSTR lpPath = (LPSTR)(LPCTSTR)sPath;
		HKEY hkey;
		DWORD cbData = MAX_PATH;
		if ( ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"), 0, KEY_ALL_ACCESS, &hkey))
    {
      AfxMessageBox("����ע��ʧ��!");
    }
    else 
    {
			LONG lnRet;
			if (isAdd)
			{
				lnRet = RegSetValueEx(hkey,	_T("MyTask"),	0, REG_SZ, (CONST BYTE *)lpPath, cbData);
				if(ERROR_SUCCESS == lnRet)
					AfxMessageBox("���ÿ��������ɹ���");
				else
				{
					CString st;
					st.Format("���ÿ�������ʧ��[%d]��",lnRet);
					AfxMessageBox(st);
				}
			}
			else
			{
				lnRet = RegDeleteValue(hkey, _T("MyTask"));
				if(ERROR_SUCCESS == lnRet)
					AfxMessageBox("ȡ�����������ɹ���");
				else
				{
					CString st;
					st.Format("ȡ����������ʧ��[%d]��",lnRet);
					AfxMessageBox(st);
				}
			}
			
    }
    RegCloseKey(hkey);
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
