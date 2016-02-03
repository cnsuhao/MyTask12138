#if !defined(AFX_TYPES_H__17436_8344_8645_7890_F1455117262D__INCLUDED_)
#define AFX_TYPES_H__17436_8344_8645_7890_F1455117262D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(push, 1)

#define VER_HIGH (0x01)

// v1.4
//#define VER_LOW  (0x04)

// v1.5新增功能：
    // ① ★添加单实例，多次启动自动唤醒第一个实例的功能
    // ② ★任务拷贝与粘贴功能
    // ③ ★托盘图标自动重建功能(2012-12-13)
    // ④ ★图盘闪烁功能，类似QQ(2012-12-14)
//#define VER_LOW  (0x05)

// v1.6新增功能：
	// ① ★托盘气球提醒功能(2012-12-14)
//#define VER_LOW  (0x06)

// v1.7新增功能：
    // ① ★日常提醒功能(2015-09-11)
    // ② ★日志规整(2015-09-12)
    // ③ ★添加文本配置支持(2015-09-12)
    // ④ ★任务排序功能(2015-11-03)
    // ⑤ ★任务xml格式导出(2015-11-13)
    // ⑥ ★日常任务xml格式导出(2015-11-17)
    // ⑦ ★版本信息展示(2015-11-20)
    // ⑧ ★过期提醒统一处理(2016-01-16)
    // ⑨ ★日常任务xml格式导入(2016-01-16)
    // ⑩ ★编译告警消除(2016-01-17)
//#define VER_LOW  (0x07)

// v1.8 新增功能：
//    ① ★根据屏幕分辨率修改按钮位置和大小
//    ② ★消息提醒强制最前端
//    ③ ★消息框加上图标
//    ④ ★配置文件编辑按钮
//    ⑤ ★天气预报功能，支持多城市配置
//    ⑥ ★重要日期展示
//    ⑦ ★CppCheck代码优化

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
#define MAIN_DLG_NAME _T("我的任务管理")

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
// p: 缓冲区指针
// i: 缓冲区下标
// a: 任意基础类型、引用类型和类类型的数据（不能是指针）
#define COPY(p,i,a) { memcpy(p+i, &a, sizeof(a)); i+=sizeof(a);}
#define COPYBAK(p,i,a) { memcpy(&a, p+i, sizeof(a)); i+=sizeof(a);}
// p: 缓冲区指针
// i: 缓冲区下标
// a: char*或者UINT8*类型的指针
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
	MT_UNDEFINED,  // 未定义
	MT_COMMON = 1,		 // 普通任务
    MT_REPEAT = 2,
    MT_TIMING = 4,
    MT_CMD = 8,
    MT_RMD = 16,
	MT_REPEAT_RMD = MT_REPEAT|MT_RMD, // 循环提醒
	MT_TIMING_RMD = MT_TIMING|MT_RMD, // 定时提醒
	MT_REPEAT_CMD = MT_REPEAT|MT_CMD, // 循环执行（脚本或程序）
	MT_TIMING_CMD = MT_TIMING|MT_CMD, // 定时执行（脚本或程序）
    MT_REPEAT_RCMD = MT_REPEAT|MT_RMD|MT_CMD, // 定时提醒并执行脚本
    MT_TIMING_RCMD = MT_TIMING|MT_RMD|MT_CMD, // 定时提醒并执行脚本
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
	TT_CO_START = 0, // 普通事务开始
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

// 提示时间类型
enum TYPE_RMD_TIME
{
	RMD_TT_PER_DAY,      // 每天提醒一次
	RMD_TT_PER_WEEK,     // 每周提醒一次
	RMD_TT_PER_MONTH,    // 每月提醒一次
	RMD_TT_DAYS,         // 每隔X天提醒一次
	RMD_TT_EXACT_TIME,   // 提醒一次
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
*	  日期选择框的特殊选项
*/
enum tagSelectIndex
{
    SI_ALL = 0,         // 所有任务
    SI_TODAY,           // 今日任务
    SI_NON_FINISHED,    // 未完成任务
    DEFAULT_SELECT_DATE // 普通日期开始
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
        // 用于普通的任务：约定结束时间
        __time32_t tiEndSet;
        // 用于定时任务：提醒时间类型
        struct RmdType
        {
            short nTypeTime;
            short nDays;
        }rt;
        // 用于循环任务：提醒时间间隔
        __time32_t tiSpan;
    }tipNext;
    int nMusicIndex;

    union
    {
        // 用于普通任务：任务的真正完成时间
        __time32_t tiEnd;
        // 用于循环或定时任务：下次提醒时间
        __time32_t tiNextRmd;
    };

    int nPercent;
    int nIsTipStarted;
    int nIsTipEnded;
    __time32_t tiExtraRmd;
    int nIsTipExtra;

    // v1.5版本的数据
    int nEnable; // 是否启用
    int nMsgType;// 任务类型

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
	// 用户设置
	char stTitle[32];
	char stCmd[128];
	char stDetails[128];
	time_t tiStartSet;
	union 
	{
		// 用于普通的任务：约定结束时间
		time_t tiEndSet;
		// 用于定时任务：提醒时间类型
		struct RmdType
		{
			short nTypeTime;
			short nDays;
		}rt;
		// 用于循环任务：提醒时间间隔
		time_t tiSpan;
	}tipNext;
	int nMusicIndex;

	union
	{
		// 用于普通任务：任务的真正完成时间
		time_t tiEnd;
		// 用于循环或定时任务：下次提醒时间
		time_t tiNextRmd;
	};

	int nPercent;
	int nIsTipStarted;
	int nIsTipEnded;
	time_t tiExtraRmd;
	int nIsTipExtra;

	// v1.5版本的数据
	int nEnable; // 是否启用
	int nMsgType;// 任务类型
			// MT_UNDEFINED,  // 未定义
			// MT_COMMON,		  // 普通任务
			// MT_REPEAT_RMD, // 循环提醒
			// MT_TIMING_RMD, // 定时提醒
			// MT_REPEAT_CMD, // 循环执行（脚本或程序）
			// MT_TIMING_CMD, // 定时执行（脚本或程序）

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

	// 背景图片
	CPCHAR GetBakPic();
	void SetBakPic(CPCHAR name);
	bool IsBakPicSet();

	// 日志查看
	CPCHAR GetLogViewer();
	void SetLogViewer(CPCHAR name);
	bool IsLogViewerSet();

	// 开机启动
	bool IsRunAtStartUp();
	void SetRunAtStartUp(BOOL isRun = TRUE);

	// 扩展提醒方式
#define MAX_TRAY_TIP_TYPE 1
	int GetTrayTipType();
	void SetTrayTipType(int type);

	// 配置保存与读取
	int     GetSize();
	bool    SetData(UINT8* pData, int iLen);
	bool    GetData(UINT8* pData, int iLen);
	
private:
	int m_nVersion;
	// 默认文件名
	int m_nFileType;
	char m_stFileName[MAX_FILE_NAME];
	CString m_stBuf;

	// 音乐设置
	int m_nMaxMusicCount;
	int m_nMusicCount;
	char (* m_pstMusicNames)[MAX_FILE_NAME];

// 	// 工作时间段设置
// 	int m_nTimeStart;
// 	int m_nTimeEnd;

	// 显示颜色设置
	COLORREF m_nColor[MAX_COLOR];

	// 用户定义背景
	int m_nIsDefineBakGround;
	char m_stFileBakGround[MAX_FILE_NAME];

	// 日志查看
	int m_nIsSetLogViewer;
	char m_stLogViewer[MAX_FILE_NAME];

	// 是否开机自动启动
	int m_nIsRunAtStartUp;

	// 扩展提醒类型
	int m_nTrayTipType;

	// 为后面扩充做铺垫
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

// 公共方法
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