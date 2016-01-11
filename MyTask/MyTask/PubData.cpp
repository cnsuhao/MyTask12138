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
    "全选",
    "标题", 
    "起始时间", 
    "终止时间", 
    "进度",
    "完成时间", 
    "详细信息",
    "执行命令"
};
const char * CPubData::gstDefaultListTitles[] =
{
    "全选",
    "标题",        // 1
    "起始时间",    // 2
    "下一次时间",  // 3
    "循环方式",    // 4
    "提醒时间",    // 5
    "详细信息",    // 6
    "提醒",        // 7
    "执行命令"     // 8
};

const char * CPubData::gstRmdTimeTypeName[]={
    "每一天",
    "每周%s",
    "每月%d日",
    "每隔%d日",
    "只提醒一次"
};

const char* CPubData::gstWeekDayName[]={
    "日","一","二","三","四","五","六"
};

const int CPubData::gnVersionModifySize = 30;
const char * CPubData::gstVersionModifyString[] = {
    "★v1.4 基本功能",
    "    ① ★任务的添加，删除和修改",
    "    ② ★任务提醒",
    "    ③ ★任务的保存与加载",
    "    ④ ★支持自选背景图片",
    "    ⑤ ★支持语音提醒",
    "    ⑥ ★任务导出到Excel功能",
    "    ⑦ ★简要日志",
    "    ⑧ ★开机自启动",
    "",
    "★v1.5 新增功能：",
    "    ① ★添加单实例，多次启动自动唤醒第一个实例的功能",
    "    ② ★任务拷贝与粘贴功能(2012-12-01)",
    "    ③ ★托盘图标自动重建功能(2012-12-13)",
    "    ④ ★图盘闪烁功能，类似QQ(2012-12-14)",
    "",
    "★v1.6 新增功能：",
    "    ① ★托盘气球提醒功能(2012-12-14)",
    "",
    "★v1.7 新增功能：",
    "    ① ★日常提醒功能(2015-09-11)",
    "    ② ★日志规整(2015-09-12)",
    "    ③ ★添加文本配置支持(2015-09-12)",
    "    ④ ★任务排序功能(2015-11-03)",
    "    ⑤ ★任务xml格式导出(2015-11-13)",
    "    ⑥ ★日常任务xml格式导出(2015-11-17)",
    "    ⑦ ★版本信息展示(2015-11-20)", 
    "    ⑧ ★过期提醒统一处理(计划)",
    "    ⑨ ★日常任务xml格式导入(计划中)", // 29
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
			// MessageBox(NULL, "已经有相同进程存在，无法启动新进程！", "提示1", MB_OK);
			CloseHandle(metuxHandle);//   关闭信号量句柄
			HWND hWndMainWindow = ::FindWindow(NULL, MAIN_DLG_NAME); //   寻找先前实例的主窗口
			if(hWndMainWindow==NULL)
			{
				::MessageBox("NULL");
			}
			::SendMessage(hWndMainWindow, MY_MSG_SHOW, NULL, NULL);//向已经存在的窗口发消息
			//::SendMessage(hWndMainWindow, WM_COPYDATA, NULL, NULL);
			exit(0);
		}
		psemObjRemind = new CSemaphore(1, 1, MUTEX_RMD_NAME);
		pRmdLock = new CSingleLock(psemObjRemind, FALSE);
		if (NULL == pRmdLock)
		{
			::MessageBox("初始化提示互斥量失败！", "提示1");
			exit(0);
		}
		if (!pRmdLock->Lock(200))
		{
			::MessageBox("初始化提示互斥量失败！", "提示2");
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
	// 加密和保存
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
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//目录不存在 
	{ 
		if (!CreateDirectory(strPath,NULL)) 
		{ 
			::MessageBox( "不能创建目录 "); 
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
	if(dwAttr==-1||(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)//目录不存在 
	{ 
		if (!CreateDirectory(strPath,NULL)) 
		{ 
			::MessageBox( "不能创建目录 "); 
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
            _T("xml文件(*.xml)|*.xml|所有文件(*.*)|*.*|"),
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
        stErrMsg = "创建内存失败!";
        return false;
    }
    if(pFileDialog->DoModal() != IDOK)   
    {
        stErrMsg = "未选择输出文件!";
        return false;   
    }
    CString cStrFile=pFileDialog->GetPathName();  //选择保存路径名称   

    stErrMsg.Format("开始导出当前视图数据到文件 [%s] ...",cStrFile);
    if(PathFileExists(cStrFile)) 
    {
        if (DeleteFile(cStrFile))
        {
            stErrMsg.Format("文件 [%s] 已存在，删除原文件成功！正在导出...",cStrFile);
        }
        else
        {
            stErrMsg.Format("文件 [%s] 已存在，删除原文件失败！",cStrFile);
            ::MessageBox(stErrMsg);
            stErrMsg.Format("导出所有数据到文件 [%s] 失败: 文件已存在且无法覆盖！",cStrFile);
            return false;
        }
    }

    stFileName = cStrFile;
    return true;
}
