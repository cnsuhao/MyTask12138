// PubData.cpp: implementation of the CPubData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTask.h"
#include "PubData.h"
#include <stdio.h>
#include "MyMessageDlg.h"
#include "MyLog.h"
#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetMsg CPubData::setMsg;
CBrush CPubData::gBr;
HANDLE CPubData::metuxHandle = NULL;
CSemaphore* CPubData::psemObjRemind = NULL;
CSingleLock * CPubData::pRmdLock = NULL;
MyTaskArray CPubData::gArrDfTasks;
LockCounter CPubData::oLockCounter;
const char * CPubData::gstListTitles[] =
{
    "ȫѡ",
    "����", 
    "��ʼʱ��", 
    "��ֹʱ��", 
    "����",
    "���ʱ��", 
    "��ϸ��Ϣ",
    "ִ������"
};
const char * CPubData::gstDefaultListTitles[] =
{
    "ȫѡ",
    "����",        // 1
    "��ʼʱ��",    // 2
    "��һ��ʱ��",  // 3
    "ѭ����ʽ",    // 4
    "����ʱ��",    // 5
    "��ϸ��Ϣ",    // 6
    "����",        // 7
    "ִ������"     // 8
};

const char * CPubData::gstRmdTimeTypeName[]={
    "ÿһ��",
    "ÿ��%s",
    "ÿ��%d��",
    "ÿ��%d��",
    "ֻ����һ��"
};

const char* CPubData::gstWeekDayName[]={
    "��","һ","��","��","��","��","��"
};

const int CPubData::gnVersionModifySize = 30;
const char * CPubData::gstVersionModifyString[] = {
    "��v1.4 ��������",
    "    �� ���������ӣ�ɾ�����޸�",
    "    �� ����������",
    "    �� ������ı��������",
    "    �� ��֧����ѡ����ͼƬ",
    "    �� ��֧����������",
    "    �� �����񵼳���Excel����",
    "    �� ���Ҫ��־",
    "    �� �￪��������",
    "",
    "��v1.5 �������ܣ�",
    "    �� ����ӵ�ʵ������������Զ����ѵ�һ��ʵ���Ĺ���",
    "    �� �����񿽱���ճ������(2012-12-01)",
    "    �� ������ͼ���Զ��ؽ�����(2012-12-13)",
    "    �� ��ͼ����˸���ܣ�����QQ(2012-12-14)",
    "",
    "��v1.6 �������ܣ�",
    "    �� �������������ѹ���(2012-12-14)",
    "",
    "��v1.7 �������ܣ�",
    "    �� ���ճ����ѹ���(2015-09-11)",
    "    �� ����־����(2015-09-12)",
    "    �� ������ı�����֧��(2015-09-12)",
    "    �� ������������(2015-11-03)",
    "    �� ������xml��ʽ����(2015-11-13)",
    "    �� ���ճ�����xml��ʽ����(2015-11-17)",
    "    �� ��汾��Ϣչʾ(2015-11-20)", 
    "    �� ���������ͳһ����(�ƻ�)",
    "    �� ���ճ�����xml��ʽ����(�ƻ���)", // 29
    "" // 30
};

CPubData::CPubData()
{

}

CPubData::~CPubData()
{

}

bool CPubData::InitData()
{
	if (NULL == metuxHandle)
	{
		metuxHandle = CreateMutex(NULL,FALSE,MUTEX_NAME);
		DWORD dwRet = WaitForSingleObject(metuxHandle, 200);
		if (WAIT_TIMEOUT == dwRet || WAIT_OBJECT_0 != dwRet)
		{
			// MessageBox(NULL, "�Ѿ�����ͬ���̴��ڣ��޷������½��̣�", "��ʾ1", MB_OK);
			CloseHandle(metuxHandle);//   �ر��ź������
			HWND hWndMainWindow = ::FindWindow(NULL, MAIN_DLG_NAME); //   Ѱ����ǰʵ����������
			if(hWndMainWindow==NULL)
			{
				::MessageBox("NULL");
			}
			::SendMessage(hWndMainWindow, MY_MSG_SHOW, NULL, NULL);//���Ѿ����ڵĴ��ڷ���Ϣ
			//::SendMessage(hWndMainWindow, WM_COPYDATA, NULL, NULL);
			exit(0);
		}
		psemObjRemind = new CSemaphore(1, 1, MUTEX_RMD_NAME);
		pRmdLock = new CSingleLock(psemObjRemind, FALSE);
		if (NULL == pRmdLock)
		{
			::MessageBox("��ʼ����ʾ������ʧ�ܣ�", "��ʾ1");
			exit(0);
		}
		if (!pRmdLock->Lock(200))
		{
			::MessageBox("��ʼ����ʾ������ʧ�ܣ�", "��ʾ2");
			exit(0);
		}
		pRmdLock->Unlock();
		//psemObjRemind->Unlock(1);
		//::ReleaseMutex(mutexRmdHandle);
	}

	FILE* fp = NULL;
	CString stFile = GetDataFileName(FILE_NAME_SET);
	fp = fopen(LPCTSTR(stFile), "rb");
	if(NULL == fp)
		return false;
	int len;

	fread(&len, sizeof(len), 1, fp);

	UINT8* pData = new UINT8[len];
	UINT8* pEData = new UINT8[len];

	fread(pData, len, 1, fp);
	EncryptData(pData, pEData, len);

	if(!setMsg.GetData(pEData, len))
	{
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

bool CPubData::SaveData()
{
	FILE* fp = NULL;
	CString stFile = GetDataFileName(FILE_NAME_SET);
	fp = fopen(LPCTSTR(stFile), "wb");
	if(NULL == fp)
		return false;
	int tmplen = setMsg.GetSize();
	int len = ParseSize16(tmplen);
	UINT8* pData = new UINT8[len];
	UINT8* pEData = new UINT8[len];
	if (!setMsg.SetData(pData, len))
	{
		delete []pData;
		delete []pEData;
		fclose(fp);
		return false;
	}
	// ...
	// ���ܺͱ���
	EncryptData(pData, pEData, len);
	int k = fwrite(&len, sizeof(len), 1, fp);
	k = fwrite(pEData, 1, len, fp);
	k = fflush(fp);

	delete []pData;
	delete []pEData;
	fclose(fp);
	return true;
}

CString CPubData::GetDataFileName( const char*stName )
{
	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath + "\\dat\\";
	DWORD   dwAttr   =   GetFileAttributes(strPath); 
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//Ŀ¼������ 
	{ 
		if (!CreateDirectory(strPath,NULL)) 
		{ 
			::MessageBox( "���ܴ���Ŀ¼ "); 
			CString stTmp = ".\\";
			return stTmp + stName; 
		} 
	}
	return strPath + stName;
}

CString CPubData::GetLogFileName( const char*stName )
{
	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	strPath = strPath + "\\log\\";
	DWORD   dwAttr   =   GetFileAttributes(strPath); 
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//Ŀ¼������ 
	{ 
		if (!CreateDirectory(strPath,NULL)) 
		{ 
			::MessageBox( "���ܴ���Ŀ¼ "); 
			CString stTmp = ".\\";
			return stTmp + stName; 
		} 
	}
	return strPath + stName;
}

void CPubData::Destroy()
{
	if (metuxHandle != NULL)
	{
		CloseHandle(metuxHandle);
		metuxHandle = NULL;
	}
}

CString CPubData::FormatDate(time_t ti)
{
	CTime tiTm(ti*3600*24);
	return tiTm.Format("%Y-%m-%d");
}

CString CPubData::GetFileTypeName()
{
	return setMsg.GetFileTypeName();
}

CString CPubData::GetCurrentDir()
{
	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	return strPath;
}

CString CPubData::GetPureName( const CString& m_stLogViewer, BOOL bIsExt /*= FALSE */ )
{
	int i=m_stLogViewer.GetLength()-1, j=0,k=0;
	while(m_stLogViewer[i] != '.') i--;
	k = i;
	while(m_stLogViewer[i] != '\\') i--;
	j = i + 1;

	if (!bIsExt)
		return m_stLogViewer.Mid(j, k-j);
	else
		return m_stLogViewer.Mid(j);
}

CString CPubData::GetPureModuleName()
{
	CString strPath;
	::GetModuleFileName( NULL, strPath.GetBuffer(MAX_PATH), MAX_PATH );
	strPath.ReleaseBuffer();
	return GetPureName(strPath,FALSE);
}

int MessageBox( LPCSTR lpText, LPCSTR lpCaption/*=""*/, UINT nSeconds/*=10*/, BOOL bDefaultOnOK/*=TRUE*/)
{
	CMyMessageDlg dlg(lpText, lpCaption, nSeconds, bDefaultOnOK, NULL);
	return dlg.DoModal();
}

bool CheckDate( const char* stDay, time_t * ptiDate /* = NULL */)
{
    if(stDay == NULL)
    {
        return false;
    }
    struct tm tmDay;
    memset(&tmDay, 0, sizeof(tmDay));
    if (strchr(stDay, '-') != NULL)
    {
        sscanf(stDay, "%d-%d-%d", 
            &tmDay.tm_year, &tmDay.tm_mon, & tmDay.tm_mday);
    }
    else if (strchr(stDay, '/') != NULL)
    {
        sscanf(stDay, "%d/%d/%d", 
            &tmDay.tm_year, &tmDay.tm_mon, & tmDay.tm_mday);
    }
    else
    {
        sscanf(stDay, "%4d%2d%2d",
            &tmDay.tm_year, &tmDay.tm_mon, & tmDay.tm_mday);
    }
    ADD_DEBUG("stDay=[%s], tmDay=[%4d-%02d-%02d]",
        stDay, tmDay.tm_year, tmDay.tm_mon, tmDay.tm_mday);
    if (!_CheckDate(tmDay, stDay))
    {
        return false;
    }
    tmDay.tm_year -= 1900;
    tmDay.tm_mon -= 1;
    time_t ti = mktime(&tmDay);
    if (ti == -1)
    {
        ADD_ERROR("stDay=[%s] mktime error", stDay);
        return false;
    }
    else
    {
        if (ptiDate != NULL)
        {
            *ptiDate = ti;
        }
        ADD_DEBUG("stDay=[%s] return TRUE",stDay);
        return true;
    }
}

bool _CheckDate( struct tm & tmDay, const char* stDay)
{
    if (tmDay.tm_mon < 1 || tmDay.tm_mon > 12)
    {
        ADD_ERROR("stDay=[%s] Check Mon[%d] FALSE", stDay, tmDay.tm_mon);
        return false;
    }
    if (tmDay.tm_year < 1900)
    {
        ADD_ERROR("stDay=[%s] Check Year[%d] FALSE", stDay, tmDay.tm_year);
        return false;
    }
    int DaysOfMonth[] =
    { 31, 28, 31,
      30, 31, 30,
      31, 31, 30,
      31, 30, 31
    };
    if ( (tmDay.tm_year % 4 == 0 && tmDay.tm_year % 100 != 0)
        || (tmDay.tm_year % 400 == 0))
    {
        DaysOfMonth[1] = 29;
    }

    if (tmDay.tm_mday < 0 || tmDay.tm_mday > DaysOfMonth[tmDay.tm_mon-1])
    {
        ADD_ERROR("stDay=[%s] Check Day[%d] FALSE", stDay, tmDay.tm_mon);
        return false;
    }

    return true;
}

bool GetExportFileName( TagFileType type_, CString& stFileName, CString& stErrMsg, CDialog* pDlg /*= NULL*/)
{
    CFileDialog* pFileDialog = NULL;
    switch(type_)
    {
    case FT_XML:
        pFileDialog = new CFileDialog(FALSE,
            _T("xml"),
            NULL,
            OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
            _T("xml�ļ�(*.xml)|*.xml|�����ļ�(*.*)|*.*|"),
            pDlg);
        break;
    case FT_EXCEL:
        pFileDialog = new CFileDialog(FALSE,
            _T("xls"),
            NULL,
            OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
            _T("Microsoft Excel(*.xls)|*.xls|"),
            pDlg);   
        break;
    default:
        break;
    }
    if (NULL == pFileDialog)
    {
        stErrMsg = "�����ڴ�ʧ��!";
        return false;
    }
    if(pFileDialog->DoModal() != IDOK)   
    {
        stErrMsg = "δѡ������ļ�!";
        return false;   
    }
    CString cStrFile=pFileDialog->GetPathName();  //ѡ�񱣴�·������   

    stErrMsg.Format("��ʼ������ǰ��ͼ���ݵ��ļ� [%s] ...",cStrFile);
    if(PathFileExists(cStrFile)) 
    {
        if (DeleteFile(cStrFile))
        {
            stErrMsg.Format("�ļ� [%s] �Ѵ��ڣ�ɾ��ԭ�ļ��ɹ������ڵ���...",cStrFile);
        }
        else
        {
            stErrMsg.Format("�ļ� [%s] �Ѵ��ڣ�ɾ��ԭ�ļ�ʧ�ܣ�",cStrFile);
            ::MessageBox(stErrMsg);
            stErrMsg.Format("�����������ݵ��ļ� [%s] ʧ��: �ļ��Ѵ������޷����ǣ�",cStrFile);
            return false;
        }
    }

    stFileName = cStrFile;
    return true;
}
