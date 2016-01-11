// MyLog.cpp: implementation of the CMyLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTask.h"
#include "MyLog.h"
#include "Types.h"
#include "PubData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLog* CMyLog::pLog = NULL;
const static char* gLogLevelName[]=
{
    "ERROR",
    "NORML",
    "TRACE",
    "DEBUG"
};

class CMyLogMaster
{
public:
	CMyLogMaster(){}
	~CMyLogMaster()
	{
		CMyLog::DeleteAll();
	}
};

static CMyLogMaster g_myLogMaster;

CMyLog::CMyLog()
{
	m_bInited = false;
}

CMyLog::~CMyLog()
{
	fout.flush();
	fout.close();
	m_bInited = false;
	m_stFileName = "";
}

CMyLog* CMyLog::GetLogPointer()
{
	if (NULL == pLog)
	{
		pLog = new CMyLog();
	}
	return pLog;
}

bool CMyLog::Init( CString stName/*=""*/ )
{
	if (m_bInited)
	{
		fout.close();
		m_bInited = false;
	}
	if (stName.IsEmpty())
	{
		CString stTmp;
		CTime ti = CTime::GetCurrentTime();
		int i,j,k;
		switch(CPubData::setMsg.GetDefaultFileType())
		{
		case FNT_BY_DAY:
			stTmp = ti.Format("LOG_DAY_%Y_%m_%d.log");
			break;
		case FNT_BY_WEEK:
			stTmp = ti.Format("LOG_WEEK_%Y_%U.log");
			break;
		case FNT_BY_MONTH:
			stTmp = ti.Format("LOG_MONTH_%Y_%m.log");
			break;
		case FNT_USER_DEFINED:
			stTmp = CPubData::setMsg.GetDefaultFileName();
			i = stTmp.GetLength()-1;
			while(stTmp[i] != '.') i--;
			j=i;
			while(stTmp[i] != '\\') i--;
			k=i;
			stTmp = stTmp.Mid(k+1, j-k-1);
			stTmp += ".log";
			break;
		default:
			m_stFileName.Empty();
			break;
		}
		m_stFileName = CPubData::GetLogFileName(LPCTSTR(stTmp));
	}
	else
	{
		m_stFileName = CPubData::GetLogFileName(LPCTSTR(stName));
	}

	fout.open(LPCTSTR(m_stFileName), ios::app);
	if(fout.is_open())
	{
		m_bInited = true;
		return true;
	}
	else
	{
		m_bInited = false;
		return false;
	}
}

bool CMyLog::Log(int _level, const char* _file, int _line, const char* fmt, ...)
{
    if(!m_bInited)
    {
        Init();
    }
    char sLogTemp[1024]={0};
    va_list ap;
    va_start (ap, fmt);
    vsnprintf(sLogTemp, sizeof(sLogTemp), fmt, ap);
    va_end (ap);

    fout<< "[" <<LPCTSTR(GetDateTime(time(NULL))) << "]["
        << gLogLevelName[_level] << "] "
        //<< _file << ":" << _line << "] " 
        << sLogTemp << endl;
    return true;
}

void CMyLog::DeleteAll()
{
	if (pLog != NULL)
	{
		delete pLog;
		pLog = NULL;
	}
}

CString CMyLog::GetLogFileName()
{
	if(!m_bInited)
	{
		Init();
	}
	return m_stFileName;
}
