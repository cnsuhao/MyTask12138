#if !defined(AFX_TYPES_H__17436_8344_8645_7890_F1455117262D__INCLUDED_)
#define AFX_TYPES_H__17436_8344_8645_7890_F1455117262D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(push, 1)

#define VER_HIGH (0x01)

// v1.4
//#define VER_LOW  (0x04)

// v1.5�������ܣ�
    // �� ����ӵ�ʵ������������Զ����ѵ�һ��ʵ���Ĺ���
    // �� �����񿽱���ճ������
    // �� ������ͼ���Զ��ؽ�����(2012-12-13)
    // �� ��ͼ����˸���ܣ�����QQ(2012-12-14)
//#define VER_LOW  (0x05)

// v1.6�������ܣ�
	// �� �������������ѹ���(2012-12-14)
//#define VER_LOW  (0x06)

// v1.7�������ܣ�
    // �� ���ճ����ѹ���(2015-09-11)
    // �� ����־����(2015-09-12)
    // �� ������ı�����֧��(2015-09-12)
    // �� ������������(2015-11-03)
    // �� ������xml��ʽ����(2015-11-13)
    // �� ���ճ�����xml��ʽ����(2015-11-17)
    // �� ��汾��Ϣչʾ(2015-11-20)
    // �� ���������ͳһ����(2016-01-16)
    // �� ���ճ�����xml��ʽ����(2016-01-16)
    // �� �����澯����(2016-01-17)
//#define VER_LOW  (0x07)

// v1.8 �������ܣ�
//    �� �������Ļ�ֱ����޸İ�ťλ�úʹ�С
//    �� ����Ϣ����ǿ����ǰ��
//    �� ����Ϣ�����ͼ��
//    �� �������ļ��༭��ť
//    �� ������Ԥ�����ܣ�֧�ֶ��������
//    �� ����Ҫ����չʾ
//    �� ��CppCheck�����Ż�

#define VER_LOW  (0x08)

#define COLOR_TIME  (RGB(192,0,0))

#define MAX_COLOR 5

typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef unsigned char  UINT8;

#define MYWM_NOTIFYICON (WM_USER + 1001)
#define MY_MSG_SHOW (WM_USER + 1002)

#define MAX_FILE_NAME 512
#define SET_NAME_SPACE "Comet"
#define MAIN_DLG_NAME _T("�ҵ��������")

#ifdef _DEBUG 
  #define MUTEX_NAME "MUTEX_MY_TASK_WIN_DEBUG"
  #define MUTEX_RMD_NAME "MUTEX_MY_TASK_REMIND_DEBUG"
#else
  #define MUTEX_NAME "MUTEX_MY_TASK_WIN"
  #define MUTEX_RMD_NAME "MUTEX_MY_TASK_REMIND"
#endif

#define KEY 12345
#define SIZEOF16(a) ((sizeof(a)%16) ? (sizeof(a)/16+1)*16 : sizeof(a))
#define ParseSize16(a) ((a%16) ? (a/16+1)*16 : a)
// p: ������ָ��
// i: �������±�
// a: ����������͡��������ͺ������͵����ݣ�������ָ�룩
#define COPY(p,i,a) { memcpy(p+i, &a, sizeof(a)); i+=sizeof(a);}
#define COPYBAK(p,i,a) { memcpy(&a, p+i, sizeof(a)); i+=sizeof(a);}
// p: ������ָ��
// i: �������±�
// a: char*����UINT8*���͵�ָ��
#define COPYSTR(p,i,str) { \
	int tmpLen; \
	tmpLen = strlen(str) + 1; \
	COPY(p, i, tmpLen); \
	memcpy(p+i, str, tmpLen); \
	i += tmpLen; \
}
#define COPYSTRBAK(p,i,str) { \
	int tmpLen; \
	COPYBAK(p, i, tmpLen); \
	memcpy(str, p+i, tmpLen); \
	i += tmpLen; \
}

#define SET_STRING(a, b) {\
    if ((b) != NULL) \
    { \
      memccpy((a), (b), '\0', sizeof(a));\
      (a)[sizeof(a)-1] = '\0';\
    }\
    else\
    {\
      memset((a), 0, sizeof(a));\
    }\
}

#define SAFE_DELETE(p) \
    if ((p) != NULL) \
    {\
      delete (p);\
      (p) = NULL;\
    }

enum MsgType
{
	MT_UNDEFINED,  // δ����
	MT_COMMON = 1,		 // ��ͨ����
    MT_REPEAT = 2,
    MT_TIMING = 4,
    MT_CMD = 8,
    MT_RMD = 16,
	MT_REPEAT_RMD = MT_REPEAT|MT_RMD, // ѭ������
	MT_TIMING_RMD = MT_TIMING|MT_RMD, // ��ʱ����
	MT_REPEAT_CMD = MT_REPEAT|MT_CMD, // ѭ��ִ�У��ű������
	MT_TIMING_CMD = MT_TIMING|MT_CMD, // ��ʱִ�У��ű������
    MT_REPEAT_RCMD = MT_REPEAT|MT_RMD|MT_CMD, // ��ʱ���Ѳ�ִ�нű�
    MT_TIMING_RCMD = MT_TIMING|MT_RMD|MT_CMD, // ��ʱ���Ѳ�ִ�нű�
};

enum TagFileType
{
    FT_XML,
    FT_EXCEL
}; 

bool CheckMsgTypeTiming(int type_);
bool CheckMsgTypeRepeat(int type_);
bool CheckMsgTypeRemind(int type_);
bool CheckMsgTypeRunCmd(int type_);
time_t GetTimeFromString(const char* stTime);

enum TypeMsgDlgType
{
	MDT_NEW,
	MDT_EDIT,
	MDT_SHOW
};

enum TypeTipType
{
	TT_CO_START = 0, // ��ͨ����ʼ
	TT_CO_END,
	TT_CO_EXTRA,
    TT_CO_FIXTIME,
    TT_CO_CIRCUL,

	TT_COUNT,
};

enum TypeExport
{
	ET_EXPORT_NONE = 0,
	ET_EXPORT_CUR,
	ET_EXPORT_ALL,
	ET_EXPORT_BY_DAY
};

enum TYPE_FILE_NAME
{
	FNT_BY_DAY,
	FNT_BY_WEEK,
	FNT_BY_MONTH,
	FNT_USER_DEFINED
};

enum TYPE_COLOR
{
	CO_UNDEFINED,
	CO_NO_TASK,
	CO_NOT_STARTED,
	CO_STARTED,
	CO_FINISHED
};

enum NUM_TIMER
{
	TIMER_UNDEFINED = 0,
	TIMER_REMIND,
	TIMER_SAVE,
	TIMER_TOP,
	TIMER_TRAY_ICON
};

// ��ʾʱ������
enum TYPE_RMD_TIME
{
	RMD_TT_PER_DAY,      // ÿ������һ��
	RMD_TT_PER_WEEK,     // ÿ������һ��
	RMD_TT_PER_MONTH,    // ÿ������һ��
	RMD_TT_DAYS,         // ÿ��X������һ��
	RMD_TT_EXACT_TIME,   // ����һ��
    RMD_TT_MAX
};

enum RMD_TIME_TYPE
{
	RMD_NOT_REMIND = 0,
	RMD_5_MIN,
	RMD_10_MIN,
	RMD_30_MIN,
	RMD_1_HOUR,
	RMD_USER_DEFINE
};

/**
*	@brief	
*	  ����ѡ��������ѡ��
*/
enum tagSelectIndex
{
    SI_ALL = 0,         // ��������
    SI_TODAY,           // ��������
    SI_NON_FINISHED,    // δ�������
    DEFAULT_SELECT_DATE // ��ͨ���ڿ�ʼ
};

typedef struct tagFileHead
{
	UINT16 Version;
	UINT16 HeadCheckSum;
	UINT16 CheckSum;
	UINT16 HeadLen;

	UINT32 PubSecret;
	UINT32 RecordCount;
	UINT16 RecordLen;
	UINT16 ReservedS1;
	UINT32 ReservedL1;

	tagFileHead();
	UINT16 GetCheckSum();
	void SetKey( UINT16 key );
	UINT16 GetKey();
}FileHead, *PFileHead;

typedef struct tagTaskMsgTime32
{
public:
    char stTitle[32];
    char stCmd[128];
    char stDetails[128];
    __time32_t tiStartSet;
    union 
    {
        // ������ͨ������Լ������ʱ��
        __time32_t tiEndSet;
        // ���ڶ�ʱ��������ʱ������
        struct RmdType
        {
            short nTypeTime;
            short nDays;
        }rt;
        // ����ѭ����������ʱ����
        __time32_t tiSpan;
    }tipNext;
    int nMusicIndex;

    union
    {
        // ������ͨ����������������ʱ��
        __time32_t tiEnd;
        // ����ѭ����ʱ�����´�����ʱ��
        __time32_t tiNextRmd;
    };

    int nPercent;
    int nIsTipStarted;
    int nIsTipEnded;
    __time32_t tiExtraRmd;
    int nIsTipExtra;

    // v1.5�汾������
    int nEnable; // �Ƿ�����
    int nMsgType;// ��������

    tagTaskMsgTime32()
    {
        strcpy(stTitle, "");
        tiStartSet = time(NULL) + 60 * 5;
        tipNext.tiEndSet = tiStartSet + 60 * 5;
        tiExtraRmd = 0;
        nMusicIndex = 0;
        strcpy(stCmd, "");
        strcpy(stDetails, "");
        nPercent = 0;
        tiEnd = 0;
        nIsTipStarted = 0;
        nIsTipEnded = 0;
        nIsTipExtra = 1;
        nMsgType = MT_COMMON;
    }
}TaskMsgTime32;

typedef struct tagTaskMsg
{
	// �û�����
	char stTitle[32];
	char stCmd[128];
	char stDetails[128];
	time_t tiStartSet;
	union 
	{
		// ������ͨ������Լ������ʱ��
		time_t tiEndSet;
		// ���ڶ�ʱ��������ʱ������
		struct RmdType
		{
			short nTypeTime;
			short nDays;
		}rt;
		// ����ѭ����������ʱ����
		time_t tiSpan;
	}tipNext;
	int nMusicIndex;

	union
	{
		// ������ͨ����������������ʱ��
		time_t tiEnd;
		// ����ѭ����ʱ�����´�����ʱ��
		time_t tiNextRmd;
	};

	int nPercent;
	int nIsTipStarted;
	int nIsTipEnded;
	time_t tiExtraRmd;
	int nIsTipExtra;

	// v1.5�汾������
	int nEnable; // �Ƿ�����
	int nMsgType;// ��������
			// MT_UNDEFINED,  // δ����
			// MT_COMMON,		  // ��ͨ����
			// MT_REPEAT_RMD, // ѭ������
			// MT_TIMING_RMD, // ��ʱ����
			// MT_REPEAT_CMD, // ѭ��ִ�У��ű������
			// MT_TIMING_CMD, // ��ʱִ�У��ű������

	tagTaskMsg()
	{
		strcpy(stTitle, "");
		tiStartSet = time(NULL) + 60 * 5;
		tipNext.tiEndSet = tiStartSet + 60 * 5;
		tiExtraRmd = 0;
		nMusicIndex = 0;
		strcpy(stCmd, "");
		strcpy(stDetails, "");
		nPercent = 0;
		tiEnd = 0;
		nIsTipStarted = 0;
		nIsTipEnded = 0;
		nIsTipExtra = 1;
		nMsgType = MT_COMMON;
	}
	tagTaskMsg(char* Title, time_t start, time_t end, int mindex, int percent = 0, char * cmd = "", char * detail = "")
	{
		memset(stTitle, 0, sizeof(stTitle));
		memset(stCmd, 0, sizeof(stCmd));
		memset(stDetails, 0, sizeof(stDetails));
		strcpy(stTitle, Title);
		tiStartSet = start;
		tipNext.tiEndSet = end;
		tiExtraRmd = 0;
		tiEnd = 0;
		nMusicIndex = mindex;
		strcpy(stCmd, cmd);
		strcpy(stDetails, detail);
		nPercent = percent;
		nIsTipStarted = 0;
		nIsTipEnded = 0;
		nIsTipExtra = 1;
		nMsgType = MT_COMMON;
	}

	UINT16 GetCheckSum();
	void CountNextRmdTime(bool bIsFirst);
    void CopyFrom32( const TaskMsgTime32& task32 );
    time_t GetNextTipDay();
    bool IsBeforeSetMinute( const CTime& _tiNow );
    void SetValue( CString &stFieldName, const char* stValue );
}TaskMsg,*PTaskMsg;

int CmpTskByTitleAsc(const void*p1, const void*p2);
int CmpTskByTitleDesc(const void*p1, const void*p2);
int CmpTskByStartTimeAsc(const void*p1, const void*p2); 
int CmpTskByStartTimeDesc(const void*p1, const void*p2); 
int CmpTskByEndTimeAsc(const void*p1, const void*p2);
int CmpTskByEndTimeDesc(const void*p1, const void*p2); 
int CmpTskByPercentAsc(const void*p1, const void*p2);
int CmpTskByPercentDesc(const void*p1, const void*p2); 
int CmpTskByFinishTimeAsc(const void*p1, const void*p2);
int CmpTskByFinishTimeDesc(const void*p1, const void*p2); 


class CSetMsg
{
	#define RESERVE_LEN 252
	typedef const char* CPCHAR;
public:
	CSetMsg()
	{
		m_nVersion = VER_HIGH * 256 + VER_LOW;
		m_nFileType = -1;
		memset(m_stFileName, 0, MAX_FILE_NAME);
		m_nMaxMusicCount = 0;
		m_nMusicCount = 0;
		m_pstMusicNames = NULL;
// 		m_nTimeStart = 8*60;
// 		m_nTimeEnd = 22*60;
		m_nColor[CO_NO_TASK] = RGB(255, 255, 255);
		m_nColor[CO_NOT_STARTED] = RGB(0, 255, 0);
		m_nColor[CO_STARTED] = RGB(255, 0, 0);
		m_nColor[CO_FINISHED] = RGB(192, 192, 192);
		m_nIsDefineBakGround = 0;
		memset(m_stFileBakGround, 0, MAX_FILE_NAME);
		m_nIsSetLogViewer = 0;
		memset(m_stLogViewer, 0, MAX_FILE_NAME);
        memset(m_pReserved, 0, sizeof(m_pReserved));
		m_nIsRunAtStartUp = 0;
		m_nTrayTipType = 0;
	}
	~CSetMsg()
  {
    if(m_pstMusicNames != NULL)
      delete [] m_pstMusicNames;
  }
	bool    SetDefaultFileName(CPCHAR stname);
	CPCHAR GetDefaultFileName(int type=-1);

	int     GetDefaultFileType();
	bool    SetDefaultFileType(int type);
	CString GetFileTypeName();

	int     RemoveMusicAt(int index);
	int     AddMusic(CPCHAR stname);
	CPCHAR  GetMusicAt(int index);
	int     GetMusicCount();
	int     RemoveAllMusic();

	COLORREF GetColor(int type);
	void SetColor(int type, COLORREF color);

	// ����ͼƬ
	CPCHAR GetBakPic();
	void SetBakPic(CPCHAR name);
	bool IsBakPicSet();

	// ��־�鿴
	CPCHAR GetLogViewer();
	void SetLogViewer(CPCHAR name);
	bool IsLogViewerSet();

	// ��������
	bool IsRunAtStartUp();
	void SetRunAtStartUp(BOOL isRun = TRUE);

	// ��չ���ѷ�ʽ
#define MAX_TRAY_TIP_TYPE 1
	int GetTrayTipType();
	void SetTrayTipType(int type);

	// ���ñ������ȡ
	int     GetSize();
	bool    SetData(UINT8* pData, int iLen);
	bool    GetData(UINT8* pData, int iLen);
	
private:
	int m_nVersion;
	// Ĭ���ļ���
	int m_nFileType;
	char m_stFileName[MAX_FILE_NAME];
	CString m_stBuf;

	// ��������
	int m_nMaxMusicCount;
	int m_nMusicCount;
	char (* m_pstMusicNames)[MAX_FILE_NAME];

// 	// ����ʱ�������
// 	int m_nTimeStart;
// 	int m_nTimeEnd;

	// ��ʾ��ɫ����
	COLORREF m_nColor[MAX_COLOR];

	// �û����屳��
	int m_nIsDefineBakGround;
	char m_stFileBakGround[MAX_FILE_NAME];

	// ��־�鿴
	int m_nIsSetLogViewer;
	char m_stLogViewer[MAX_FILE_NAME];

	// �Ƿ񿪻��Զ�����
	int m_nIsRunAtStartUp;

	// ��չ��������
	int m_nTrayTipType;

	// Ϊ�����������̵�
	char m_pReserved[RESERVE_LEN];
};

class TConfig
{
public:
    typedef CMap<CString, LPCTSTR, CString, LPCTSTR> CmdMapType;
    class TConfigImpl
    {
    public:
        friend class TConfig;
    private:
        int LogLevel;
        CmdMapType MapCmd;
        bool IsTaskTitleModifiable;
        int WeatherShow;
        CString WeatherCode;
        TConfigImpl();
        ~TConfigImpl();
    };

    static TConfigImpl& GetImpl();
    static void ReadAllConfig();
    static CString GetIniName(LPCTSTR Name);

    static void SetLogLevel(int level = 0)  { GetImpl().LogLevel = level; }
    static int  GetLogLevel()  { return GetImpl().LogLevel; }
    static bool IsTaskTitleModifiable() { return GetImpl().IsTaskTitleModifiable; }
    static int  GetWeatherShow()  { return GetImpl().WeatherShow; }
    static const CString& GetWeatherCityCode() { return GetImpl().WeatherCode; }

    static int AddCmd(LPCTSTR stName, LPCTSTR stValue);
    static int GetCmdCount() { return GetImpl().MapCmd.GetCount(); }
    static CmdMapType& GetCmds() { return GetImpl().MapCmd; }
    static CString GetCmdByName(LPCTSTR stName);
};

// ��������
int EncryptPassword(UINT8 oldpwd[16],UINT8 *newpwd);

bool EncryptData(UINT8 *oldData, UINT8 * newData, size_t len);

CString GetDateTime(time_t ti);
CString GetTime(time_t ti);
CString GetDate(time_t ti);
time_t GetDateInt(time_t ti);
LPCTSTR GetNormalTaskDataByCol( CString& sBuf, const TaskMsg& tsk, int iCol );
LPCTSTR GetDefaultTaskDataByCol( CString& sBuf, const TaskMsg& tsk, int iCol );
void GetRmdTypeFromString(TaskMsg& tsk, const char* st);
#pragma pack(pop)
#endif // !defined(AFX_TYPES_H__17436_8344_8645_7890_F1455117262D__INCLUDED_) 