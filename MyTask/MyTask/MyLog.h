// MyLog.h: interface for the CMyLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLOG_H__10E98FC9_C21A_4FAA_A0EE_5501BB9FA3B6__INCLUDED_)
#define AFX_MYLOG_H__10E98FC9_C21A_4FAA_A0EE_5501BB9FA3B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
using namespace std;

enum tagLogLevel
{
    LVL_ERROR = 0,
    LVL_INFO = 1,
    LVL_TRACE = 2,
    LVL_DEBUG = 3
};

#define ADD_LOG(_LEVEL, MSG, ...) \
  if (TConfig::GetLogLevel() >= _LEVEL) \
  { \
      CMyLog::GetLogPointer()->Log(_LEVEL, __FILE__, __LINE__, MSG, ##__VA_ARGS__); \
  }

#define ADD_ERROR(MSG, ...) ADD_LOG(LVL_ERROR, MSG, ##__VA_ARGS__)
#define ADD_NORMAL(MSG, ...) ADD_LOG(LVL_INFO, MSG, ##__VA_ARGS__)
#define ADD_TRACE(MSG, ...) ADD_LOG(LVL_TRACE, MSG, ##__VA_ARGS__)
#define ADD_DEBUG(MSG, ...) ADD_LOG(LVL_DEBUG, MSG, ##__VA_ARGS__)

class CMyLog  
{
	//������̬����
public:
	static CMyLog* GetLogPointer();
	static void DeleteAll();

	//˽�о�̬����
private:
	static CMyLog* pLog; 

	//������Ա����
public:
    bool Log(int _level, const char* _file, int _line, const char* fmt, ...);
	bool Init(CString stName="");
	CString GetLogFileName();

	//˽�г�Ա����
private:
	CMyLog();
	virtual ~CMyLog();
	//˽�г�Ա����
private:
	CString m_stFileName;
	ofstream fout;
	bool m_bInited;
};
#endif // !defined(AFX_MYLOG_H__10E98FC9_C21A_4FAA_A0EE_5501BB9FA3B6__INCLUDED_)
