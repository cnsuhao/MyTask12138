// PubData.h: interface for the CPubData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBDATA_H__8C10D8BC_505A_4C78_BA87_10CD014FE80A__INCLUDED_)
#define AFX_PUBDATA_H__8C10D8BC_505A_4C78_BA87_10CD014FE80A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Types.h"
#include <afxmt.h>
#include "MyTaskArray.h"

class CDialog;
class TiXmlNode;

#define FILE_NAME_SET "userconfig.cfg"
#define FILE_NAME_DF_TASK "default.dat"

int MessageBox(LPCSTR lpText,          // 消息内容
               LPCSTR lpCaption="",     // 消息标题
               UINT uType = MB_ICONINFORMATION,  // 图标类型
                   // 目前仅支持MB_ICONINFORMATION, MB_ICONWARNING, MB_ICONERROR
               UINT nSeconds=10,        // 自动消失时间
               BOOL bDefaultOnOK=TRUE  // 默认按钮是否是"确定"
              );
bool CheckDate( const char* stDay, time_t * ptiDate = NULL);
bool GetExportFileName(TagFileType type_, CString& stFileName, CString& stErrMsg, CDialog* pDlg = NULL);
bool GetImportFileName(TagFileType type_, CString& stFileName, CString& stErrMsg, CDialog* pDlg = NULL);

bool _CheckDate( struct tm & tmDay, const char* stDay);
struct LockCounter
{
    int nLockSuccess;
    int nLockFailed;
    int nUnlock;
    void ReSet()
    {
        nLockSuccess = 0;
        nLockFailed = 0;
        nUnlock = 0;
    }
    LockCounter()
    {
        ReSet();
    }
};

class CPubData  
{
public:
	CPubData();
	virtual ~CPubData();

public:
	static CBrush gBr;
	static CString FormatDate(time_t ti);
	static void Destroy();
	static CString GetDataFileName(const char*stName);
	static CString GetCurrentDir();
	static bool InitData();
	static bool SaveData();
	static CString GetFileTypeName();
	static CString GetLogFileName( const char*stName );
	static CString GetPureName( const CString& m_stLogViewer, BOOL bIsExt = FALSE );
	static CString GetPureModuleName();
    static bool ParseXmlDataNodeAsDefaultTask(TiXmlNode* pNode);
    static char* UTF8_To_GB2312(const char* utf8);
    // data
    static TaskMsg gTask;
    static CString stFieldName;
    static CSetMsg setMsg;
	static HANDLE metuxHandle;
	static CSingleLock * pRmdLock;
    static LockCounter oLockCounter;

	static MyTaskArray gArrDfTasks;
    static const char * gstListTitles[];
    static const char * gstDefaultListTitles[];
    static const char * gstRmdTimeTypeName[];
    static const char * gstWeekDayName[];
    static const char * gstVersionModifyString[];
    static const int    gnVersionModifySize;

private:
	static CSemaphore* psemObjRemind;
};
#endif // !defined(AFX_PUBDATA_H__8C10D8BC_505A_4C78_BA87_10CD014FE80A__INCLUDED_)
