#include "StdAfx.h"
#include "Types.h"
#include "PubData.h"
#include "MyLog.h"

// HANDLE mutex = NULL;
// bool bInited = false;

/* 加密16字节的密码（不足16字节也按16字节加密） */
int EncryptPassword(UINT8 oldpwd[16],UINT8 *newpwd)
{
	int i;
	UINT8 tmp[16];
	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp,oldpwd,16);
	
// 	static bool bIsFirst = true;
// 	if (bIsFirst)
// 	{
// 		srand(KEY);
// 	}
	int* p=(int*)tmp;
	int len = sizeof(int)/sizeof(char);
	
	for(i=0; i<len; i++)
	{
		p[i] ^= KEY;
	}
	memcpy(newpwd,tmp,16);
	return 1;
}

bool EncryptData( UINT8 *oldData, UINT8 * newData, size_t len )
{
	if (len % 16 != 0)
		return false;
	for (int i=0; i<(int)(len/16); i++)
	{
		EncryptPassword(oldData+16*i, newData+16*i);
	}
	return true;
}

CString GetDate( time_t ti )
{
	return CTime(ti).Format("%Y-%m-%d");
}

time_t GetDateInt( time_t ti )
{
	return (ti + 28800) / 86400 * 86400 - 28800;
}

CString GetDateTime( time_t ti )
{
	return CTime(ti).Format("%Y-%m-%d %H:%M:%S");
}

CString GetTime( time_t ti )
{
	return CTime(ti).Format("%H:%M:%S");
}

bool CheckMsgTypeTiming( int type_ )
{
    return bool(type_ & MT_TIMING);
}

bool CheckMsgTypeRepeat( int type_ )
{
    return bool(type_ & MT_REPEAT);
}

bool CheckMsgTypeRemind( int type_ )
{
    return bool(type_ & MT_RMD);
}

bool CheckMsgTypeRunCmd( int type_ )
{
    return bool(type_ & MT_CMD);
}

int CmpTskByTitleAsc( const void*p1, const void*p2 )
{
    const PTaskMsg pt1 = (const PTaskMsg)p1;
    const PTaskMsg pt2 = (const PTaskMsg)p2;
    return strcmp(pt1->stTitle, pt2->stTitle);
}

int CmpTskByTitleDesc( const void*p1, const void*p2 )
{
    return -CmpTskByTitleAsc(p1, p2);
}


int CmpTskByStartTimeAsc( const void*p1, const void*p2 )
{
    const PTaskMsg pt1 = (const PTaskMsg)p1;
    const PTaskMsg pt2 = (const PTaskMsg)p2;
    return pt1->tiStartSet - pt2->tiStartSet;
}

int CmpTskByStartTimeDesc( const void*p1, const void*p2 )
{
    return -CmpTskByStartTimeAsc(p1, p2);
}

int CmpTskByEndTimeAsc( const void*p1, const void*p2 )
{
    const PTaskMsg pt1 = (const PTaskMsg)p1;
    const PTaskMsg pt2 = (const PTaskMsg)p2;
    return pt1->tipNext.tiEndSet - pt2->tipNext.tiEndSet;
}

int CmpTskByEndTimeDesc( const void*p1, const void*p2 )
{
    return -CmpTskByEndTimeAsc(p1, p2);
}

int CmpTskByPercentAsc( const void*p1, const void*p2 )
{
    const PTaskMsg pt1 = (const PTaskMsg)p1;
    const PTaskMsg pt2 = (const PTaskMsg)p2;
    return pt1->nPercent - pt2->nPercent;
}

int CmpTskByPercentDesc( const void*p1, const void*p2 )
{
    return -CmpTskByPercentAsc(p1, p2);
}

int CmpTskByFinishTimeAsc( const void*p1, const void*p2 )
{
    const PTaskMsg pt1 = (const PTaskMsg)p1;
    const PTaskMsg pt2 = (const PTaskMsg)p2;
    return pt1->tiEnd - pt2->tiEnd;
}

int CmpTskByFinishTimeDesc( const void*p1, const void*p2 )
{
    return -CmpTskByFinishTimeAsc(p1, p2);
}

tagFileHead::tagFileHead()
{
		HeadCheckSum = 0;
		CheckSum = 0;
		Version = VER_HIGH * 256 + VER_LOW;
		HeadLen = SIZEOF16(tagFileHead);
		SetKey(KEY);
		RecordCount = 0;
		RecordLen = 0;
		ReservedL1 = 0;
		ReservedS1 = 0;
}

UINT16 tagFileHead::GetCheckSum()
{
	UINT16 sum = 0;
	UINT8* p = (UINT8*)this;
	int i = 0;
	for (i=0; i < sizeof(FileHead); i++)
	{
		sum += p[i];
	}
	return sum;
}

void tagFileHead::SetKey( UINT16 key )
{
	PubSecret = rand()<<12 | key << 4 | rand() % 256;
}

UINT16 tagFileHead::GetKey()
{
	return (PubSecret >> 4) & 0xffff;
}

UINT16 tagTaskMsg::GetCheckSum()
{
	UINT16 sum = 0;
	UINT8* p = (UINT8*)this;
	int i = 0;
	for (i=0; i < sizeof(TaskMsg); i++)
	{
		sum += p[i];
	}
	return sum;
}

void tagTaskMsg::CountNextRmdTime( bool bIsFirst )
{
    ADD_DEBUG("nMsgType=[%d]", nMsgType);
	if (MT_UNDEFINED == nMsgType || MT_COMMON == nMsgType)
		return;
    if (tipNext.rt.nTypeTime == RMD_TT_EXACT_TIME)
    {
        tiNextRmd = tiStartSet;
        nIsTipEnded = 0;
        return;
    }
	time_t tiNow = time(NULL);
	CTime tim = tiNow;
    if (CheckMsgTypeRepeat(nMsgType))
    {
        if (bIsFirst)
        {
            tiNextRmd = tiStartSet;
        }
        while (tiNextRmd<=tiNow)
        {
            tiNextRmd += tipNext.tiSpan;
        }
    }
    if (CheckMsgTypeTiming(nMsgType))
    {
        CTime tiNextDay = GetNextTipDay();
        CTime tiS = tiStartSet;
        ADD_DEBUG("NextDay=[%s]", LPCTSTR(GetDate(tiNextDay.GetTime())));
        CTime tiNext = 
            CTime(tiNextDay.GetYear(), tiNextDay.GetMonth(), tiNextDay.GetDay(),
            tiS.GetHour(), tiS.GetMinute(), 0);
        tiNextRmd = tiNext.GetTime();
    }
}

void tagTaskMsg::CopyFrom32( const TaskMsgTime32& task32 )
{
    strcpy(stTitle, task32.stTitle);
    tiStartSet = task32.tiStartSet;
    tipNext.tiEndSet = task32.tipNext.tiEndSet;
    tiExtraRmd = task32.tiExtraRmd;
    nMusicIndex = task32.nMusicIndex;
    strcpy(stCmd, task32.stCmd);
    strcpy(stDetails, task32.stDetails);
    nPercent = task32.nPercent;
    tiEnd = task32.tiEnd;
    nIsTipStarted = task32.nIsTipStarted;
    nIsTipEnded = task32.nIsTipEnded;
    nIsTipExtra = task32.nIsTipExtra;
    nMsgType = task32.nMsgType;
}

time_t tagTaskMsg::GetNextTipDay()
{
    CTime _tiNow = CTime::GetCurrentTime();
    CTime _tiNext;
    switch(this->tipNext.rt.nTypeTime)
    {
    case RMD_TT_PER_DAY:
        if (IsBeforeSetMinute(_tiNow))
        {
            _tiNext = _tiNow;
        }
        else
        {
            _tiNext = _tiNow + CTimeSpan(1, 0, 0, 0);
        }
        break;
    case RMD_TT_PER_WEEK:
        if (_tiNow.GetDayOfWeek() > tipNext.rt.nDays 
            || (_tiNow.GetDayOfWeek() == tipNext.rt.nDays 
                  && !IsBeforeSetMinute(_tiNow)))
        {
            int k = _tiNow.GetDayOfWeek();
            _tiNext = _tiNow + CTimeSpan(tipNext.rt.nDays - _tiNow.GetDayOfWeek() + 7, 0, 0, 0);
            k = _tiNext.GetDayOfWeek();
            k++;
        }
        else
        {
            _tiNext = _tiNow + CTimeSpan(tipNext.rt.nDays - _tiNow.GetDayOfWeek(), 0, 0, 0);
        }
        break;
    case RMD_TT_PER_MONTH:
        if (_tiNow.GetDay() > tipNext.rt.nDays 
            || (_tiNow.GetDay() == tipNext.rt.nDays 
             && !IsBeforeSetMinute(_tiNow)))
        {
            _tiNext = CTime(_tiNow.GetYear(), (_tiNow.GetMonth()+1)%12, tipNext.rt.nDays,
                0, 0, 0);
        }
        else
        {
            _tiNext = CTime(_tiNow.GetYear(), _tiNow.GetMonth(), tipNext.rt.nDays,
                0, 0, 0);
        }
        break;
    case RMD_TT_DAYS:
        {
            CTime _tiLast = (tiNextRmd == 0) ? tiStartSet : tiNextRmd;
            if (_tiNow.GetDay() > _tiLast.GetDay()
                || (_tiNow.GetDay() == _tiLast.GetDay() 
                && !IsBeforeSetMinute(_tiLast)))
            {
                _tiNext = _tiLast + CTimeSpan(tipNext.rt.nDays, 0, 0, 0);
            }
            else
            {
                _tiNext = _tiLast;
            }
        }
        break;
    }
    return (_tiNext.GetTime()+8*3600)/(3600*24)*(3600*24)-8*3600;
}

bool tagTaskMsg::IsBeforeSetMinute( const CTime& tiNow )
{
    CTime tiS(tiStartSet);
    if (tiNow.GetHour() < tiS.GetHour()) return true;
    if (tiNow.GetHour() > tiS.GetHour()) return false;
    if (tiNow.GetMinute() < tiS.GetMinute())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void tagTaskMsg::SetValue( CString &stFieldName, const char* stValue )
{
    if (stFieldName == "标题")
    {
        SET_STRING(stTitle, stValue);
    }
    else if(stFieldName == "起始时间")
    {
        tiStartSet = GetTimeFromString(stValue);
    }
    else if(stFieldName == "下一次时间")
    {
        tiNextRmd = GetTimeFromString(stValue);
    }
    else if(stFieldName == "循环方式")
    {
        tipNext.rt.nTypeTime = (short)atoi(stValue);
    }
    else if(stFieldName == "提醒时间")
    {
        // Do Nothing
    }
    else if(stFieldName == "详细信息")
    {
        SET_STRING(stDetails, stValue);
    }
    else if(stFieldName == "提醒")
    {
        if (atoi(stValue) == 1)
        {
            nMsgType |= MT_RMD;
        }
    }
    else if(stFieldName == "执行命令")
    {
        if (strlen(stValue) > 0)
        {
            nMsgType |= MT_CMD;
            SET_STRING(stCmd, stValue);
        }
    }
}

bool CSetMsg::SetDefaultFileName( CPCHAR stname )
{
	memcpy(m_stFileName, stname, MAX_FILE_NAME);
	m_stFileName[MAX_FILE_NAME-1] = '\0';
	return true;
}

CSetMsg::CPCHAR CSetMsg::GetDefaultFileName(int type /* =-1 */)
{
	int realType = (type==-1 ? m_nFileType : type);
	CTime ti = CTime::GetCurrentTime();
	CString stTmp;
	switch(realType)
	{
		case FNT_BY_DAY:
			stTmp = ti.Format("DAY_%Y_%m_%d.dat");
			m_stBuf = CPubData::GetDataFileName(stTmp);
			break;
		case FNT_BY_WEEK:
			stTmp = ti.Format("WEEK_%Y_%U.dat");
			m_stBuf = CPubData::GetDataFileName(stTmp);
			break;
		case FNT_BY_MONTH:
			stTmp = ti.Format("MONTH_%Y_%m.dat");
			m_stBuf = CPubData::GetDataFileName(stTmp);
			break;
		case FNT_USER_DEFINED:
			m_stBuf = m_stFileName;
			break;
		default:
			m_stBuf.Empty();
			break;
	}
	return LPCTSTR(m_stBuf);
}

bool CSetMsg::SetDefaultFileType( int type )
{
	m_nFileType = type;
	return true;
}

int CSetMsg::GetDefaultFileType()
{
	return m_nFileType;
}

int CSetMsg::GetMusicCount()
{
	return m_nMusicCount;
}

int CSetMsg::RemoveMusicAt( int index )
{
	if (index >= m_nMusicCount)
		return -1;
	
	for (int i=index; i<m_nMusicCount-1; i++)
	{
		memcpy(m_pstMusicNames[i], m_pstMusicNames[i+1], MAX_FILE_NAME);
	}
	m_nMusicCount--;
	return m_nMusicCount;
}

CSetMsg::CPCHAR CSetMsg::GetMusicAt( int index )
{
	if (index >= m_nMusicCount)
		return NULL;

	return m_pstMusicNames[index];
}

int CSetMsg::RemoveAllMusic()
{
	m_nMusicCount = 0;
	return 0;
}

int CSetMsg::AddMusic( CPCHAR stname )
{
	if(m_nMaxMusicCount <= m_nMusicCount)
	{
		char (* pstTmp)[MAX_FILE_NAME] = NULL;
		m_nMaxMusicCount = m_nMusicCount + 10;
		pstTmp = new char [m_nMaxMusicCount][MAX_FILE_NAME];
		for (int i=0; i<m_nMusicCount; i++)
		{
			memcpy(pstTmp[i], m_pstMusicNames[i], MAX_FILE_NAME);
		}
		delete [] m_pstMusicNames;
		m_pstMusicNames = pstTmp;
	}
	m_nMusicCount++;
	memcpy(m_pstMusicNames[m_nMusicCount-1], stname, MAX_FILE_NAME);
	return m_nMusicCount;
}

bool CSetMsg::SetData( UINT8* pData, int iLen )
{
	int index = 0;
	m_nVersion = VER_HIGH * 256 + VER_LOW;
	COPY(pData, index, m_nVersion);
// 	char st[8];
// 	sprintf(st, "版本：%d", m_nVersion);
// 	::MessageBox(NULL, st, "版本", MB_OK);
	COPY(pData, index, m_nFileType);
	COPYSTR(pData, index, m_stFileName);
	COPY(pData, index, m_nColor[CO_NO_TASK]);
	COPY(pData, index, m_nColor[CO_NOT_STARTED]);
	COPY(pData, index, m_nColor[CO_STARTED]);
	COPY(pData, index, m_nColor[CO_FINISHED]);
	COPY(pData, index, m_nMusicCount);
	for(int i=0; i<m_nMusicCount; i++)
	{
		COPYSTR(pData, index, m_pstMusicNames[i]);
	}
	if( m_nVersion >= 256*1 + 3)
	{
		// v1.3以上版本
		// 添加了背景图片的控制
		COPY(pData, index, m_nIsDefineBakGround);
		COPYSTR(pData, index, m_stFileBakGround);
	}
	if( m_nVersion >= 256*1 + 4)
	{
		// v1.4以上版本
		// 自定义日志查看软件
		COPY(pData, index, m_nIsSetLogViewer);
		COPYSTR(pData, index, m_stLogViewer);

		// 是否开机启动
		COPY(pData, index, m_nIsRunAtStartUp);
		
		if (m_nVersion >= 256*1 + 5)
		{
			if(m_nTrayTipType<0 || m_nTrayTipType>MAX_TRAY_TIP_TYPE)
			{
				m_nTrayTipType = 0;
			}
			//m_nTrayTipType = 0;

			COPY(pData, index, m_nTrayTipType);
			// 写入预留字段
			memset(m_pReserved, '0', RESERVE_LEN);
			m_pReserved[RESERVE_LEN-1] = '\0';
			COPYSTR(pData, index, m_pReserved);
		}
		else
		{
			// 写入预留字段
			memset(m_pReserved, '\0', RESERVE_LEN);
			m_pReserved[RESERVE_LEN-1] = '\0';
			COPYSTR(pData, index, m_pReserved);
		}
		
	}
// 	CString st;
// 	st.Format("Index=%d",index);
// 	AfxMessageBox(st);

	return true;
}

bool CSetMsg::GetData( UINT8* pData, int iLen )
{
	int index = 0;
	COPYBAK(pData, index, m_nVersion);
	COPYBAK(pData, index, m_nFileType);
	COPYSTRBAK(pData, index, m_stFileName);
	COPYBAK(pData, index, m_nColor[CO_NO_TASK]);
	COPYBAK(pData, index, m_nColor[CO_NOT_STARTED]);
	COPYBAK(pData, index, m_nColor[CO_STARTED]);
	COPYBAK(pData, index, m_nColor[CO_FINISHED]);
	COPYBAK(pData, index, m_nMusicCount);
	m_nMaxMusicCount = (m_nMusicCount/10+1) * 10;
	m_pstMusicNames = new char[m_nMaxMusicCount][MAX_FILE_NAME];
	for(int i=0; i<m_nMusicCount; i++)
	{
		COPYSTRBAK(pData, index, m_pstMusicNames[i]);
	}
// 	char st[8];
// 	sprintf(st, "%d", m_nVersion);
	if( m_nVersion >= 256*1 + 3)
	{
		// v1.3以上版本
		// 添加了背景图片的控制
		COPYBAK(pData, index, m_nIsDefineBakGround);
		COPYSTRBAK(pData, index, m_stFileBakGround);
	}
	if( m_nVersion >= 256*1 + 4)
	{
		// v1.4以上版本
		// 自定义日志查看软件
		COPYBAK(pData, index, m_nIsSetLogViewer);
		COPYSTRBAK(pData, index, m_stLogViewer);
		// 是否开机启动
		COPYBAK(pData, index, m_nIsRunAtStartUp);

		if (m_nVersion >= 256*1 + 6)
		{
			COPYBAK(pData, index, m_nTrayTipType);
			if(m_nTrayTipType<0 || m_nTrayTipType>MAX_TRAY_TIP_TYPE)
			{
				m_nTrayTipType = 0;
			}

			// 读取预留字段
			COPYSTRBAK(pData, index, m_pReserved);
		}
		else
		{
			// 读取预留字段
			COPYSTRBAK(pData, index, m_pReserved);
		}
	}
	return true;
}

int CSetMsg::GetSize()
{
	int len= sizeof(int)*(12+m_nMusicCount) + sizeof(COLORREF)*4;
	len += strlen(m_stFileName)+1;
	for(int i=0; i<m_nMusicCount; i++)
		len += strlen(m_pstMusicNames[i])+1;

	len += strlen(m_stFileBakGround) + 1; // 背景图片
	len += strlen(m_stLogViewer) + 1;
	len += RESERVE_LEN;
// 	CString st;
// 	st.Format("GetSize=%d",len);
// 	AfxMessageBox(st);
	return len;
}

CString CSetMsg::GetFileTypeName()
{
	switch(m_nFileType)
	{
		case FNT_BY_DAY:
			return "当前按日进行保存";
		case FNT_BY_WEEK:
			return "当前按周进行保存";
		case FNT_BY_MONTH:
			return "当前按月进行保存";
		case FNT_USER_DEFINED:
			return "当前自定义文件名保存";
		default:
			return "请先进行【设置】...";
	}
}

COLORREF CSetMsg::GetColor( int type )
{
	if (type>=0 && type<MAX_COLOR)
		return m_nColor[type];
	else
		return RGB(0,0,0);
}

void CSetMsg::SetColor( int type, COLORREF color )
{
	if (type>=0 && type<MAX_COLOR)
		m_nColor[type] = color;
}

CSetMsg::CPCHAR CSetMsg::GetBakPic()
{
	return m_stFileBakGround;
}

void CSetMsg::SetBakPic( CPCHAR name )
{
	if (NULL == name)
	{
		m_nIsDefineBakGround = 0;
		return;
	}
	memcpy(m_stFileBakGround, name, MAX_FILE_NAME);
	m_nIsDefineBakGround = 1;
}

bool CSetMsg::IsBakPicSet()
{
	return m_nIsDefineBakGround != 0;
}

bool CSetMsg::IsLogViewerSet()
{
	return m_nIsSetLogViewer != 0;
}

CSetMsg::CPCHAR CSetMsg::GetLogViewer()
{
	if(m_nIsSetLogViewer)
	{
		return m_stLogViewer;
	}
	else
	{
		return NULL;
	}
}

void CSetMsg::SetLogViewer( CPCHAR name )
{
	if (NULL == name)
	{
		m_nIsSetLogViewer = 0;
		return;
	}
	memcpy(m_stLogViewer, name, MAX_FILE_NAME);
	m_nIsSetLogViewer = 1;
}

bool CSetMsg::IsRunAtStartUp()
{
	return m_nIsRunAtStartUp != 0;
}

void CSetMsg::SetRunAtStartUp( BOOL isRun /*= TRUE*/ )
{
	if (isRun)
		m_nIsRunAtStartUp = 1;
	else
		m_nIsRunAtStartUp = 0;
}

int CSetMsg::GetTrayTipType()
{
	return m_nTrayTipType;
}

void CSetMsg::SetTrayTipType( int type )
{
	if(type>=0 && type<=MAX_TRAY_TIP_TYPE)
		m_nTrayTipType = type;
}

CString TConfig::GetIniName(LPCTSTR Name)
{
    static bool bFirst = true;
    static CString path;
    if (bFirst)
    {
        CString strAppPath;
        ::GetModuleFileName(NULL, strAppPath.GetBuffer(_MAX_PATH), _MAX_PATH);
        strAppPath.ReleaseBuffer(); 
        int nPos = strAppPath.ReverseFind('\\'); 
        path = strAppPath.Left(nPos);
        bFirst = false;
    }
    CString stPathName = path + _T("\\") + Name;
    return stPathName;
}

void TConfig::ReadAllConfig()
{
    CString stFileName = GetIniName("task.ini");
    GetImpl().MapCmd.InitHashTable(32);

    GetImpl().LogLevel = GetPrivateProfileInt("LOG", "LogLevel", 0, LPCTSTR(stFileName));
    ADD_NORMAL("读取配置LogLevel=[%d]", GetImpl().LogLevel);

    GetImpl().IsTaskTitleModifiable =
        (GetPrivateProfileInt("Common", "TaskTitleModifiable", 0, LPCTSTR(stFileName))==1 ? true : false);
    ADD_NORMAL("读取配置TaskTitleModifiable=[%s]", 
        GetImpl().IsTaskTitleModifiable ? "true" : "false");

    int nCount = GetPrivateProfileInt("Cmd", "Count", 0, LPCTSTR(stFileName));
    CString stTName, stTValue;
    char stName[32]={0}, stValue[128] = {0};
    for(int i=0; i<nCount; ++i)
    {
        stTName.Format("Name%d", i+1);
        stTValue.Format("Value%d", i+1);
        (void)GetPrivateProfileString("Cmd", stTName, 
            "", stName, sizeof(stName), LPCTSTR(stFileName));
        (void)GetPrivateProfileString("Cmd", stTValue, 
            "", stValue, sizeof(stValue), LPCTSTR(stFileName));
        if (strlen(stName) == 0 || strlen(stValue) == 0)
        {
            ADD_ERROR("读取配置[Cmd][%s=%s][%s=%s]失败！",
                stTName, stName, stTValue, stValue);
            continue;
        }
        GetImpl().MapCmd.SetAt(stName, stValue);
        ADD_NORMAL("读取配置[Cmd][%s=[%s]]成功！",stName, stValue);
    }

    // 读取天气城市
    {
        GetImpl().WeatherShow = GetPrivateProfileInt("WeatherCity", "WeatherShow", 
            0, LPCTSTR(stFileName));
        ADD_NORMAL("读取配置WeatherShow=[%d]", GetImpl().WeatherShow);

        if (GetImpl().WeatherShow != 0)
        {
            char stCityName[32]={0}, stCityCode[32] = {0};

            // 读取城市名称
            (void)GetPrivateProfileString("WeatherCity", "CityName", 
                "", stCityName, sizeof(stCityName), LPCTSTR(stFileName));
            if (strlen(stCityName) == 0)
            {
                ADD_ERROR("读取配置[CityName]失败！");
                return;
            }

            // 读取城市代码
            CString stCityCodeIniName = CPubData::GetDataFileName("CityCode.ini");
            (void)GetPrivateProfileString("CityCode", stCityName, 
                "", stCityCode, sizeof(stCityCode),
                LPCTSTR(stCityCodeIniName));

            GetImpl().WeatherCode = stCityCode;
            ADD_NORMAL("读取配置WeatherCity=[%s],CityCode=[%s]",
                stCityName, stCityCode);
        }
    }
}

CString TConfig::GetCmdByName( LPCTSTR stName )
{
    CString stValue;
    if(GetImpl().MapCmd.Lookup(stName, stValue))
    {
        return stValue;
    }
    else
    {
        return "";
    }
}

TConfig::TConfigImpl& TConfig::GetImpl()
{
    static TConfigImpl _impl;
    return _impl;
}

LPCTSTR GetNormalTaskDataByCol( CString& sBuf, const TaskMsg& tsk, int iCol )
{
    switch(iCol)
    {
    case 1:
        sBuf = tsk.stTitle;
        break;
    case 2:
        {
            CTime ti(tsk.tiStartSet);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%Y-%m-%d %H:%M");
            }
            break;
        }
    case 3:
        {
            CTime ti(tsk.tipNext.tiEndSet);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%Y-%m-%d %H:%M");
            }
            break;
        }
    case 4:
        {
            sBuf.Format("%d%%", tsk.nPercent);
            break;
        }
    case 5:
        {
            CTime ti(tsk.tiEnd);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%Y-%m-%d %H:%M");
            }
            break;
        }
    case 6:
        {
            sBuf = tsk.stDetails;
            break;
        }
    case 7:
        {
            sBuf = tsk.stCmd;
            break;
        }
    default:
        {
            sBuf = "";
            break;
        }
    }

    return sBuf;
}

LPCTSTR GetDefaultTaskDataByCol( CString& sBuf, const TaskMsg& tsk, int iCol )
{
    switch(iCol)
    {
    case 1:
        sBuf = tsk.stTitle;
        break;
    case 2:
        {
            CTime ti(tsk.tiStartSet);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%Y-%m-%d %H:%M");
            }
            break;
        }
    case 3:
        {
            CTime ti(tsk.tiNextRmd);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%Y-%m-%d %H:%M");
            }
            break;
        }
    case 4:
        {
            int nType = tsk.tipNext.rt.nTypeTime;
            switch (nType)
            {
            case RMD_TT_PER_DAY:
                sBuf.Format(CPubData::gstRmdTimeTypeName[nType]);
                break;
            case RMD_TT_PER_WEEK:
                sBuf.Format(CPubData::gstRmdTimeTypeName[nType], 
                    CPubData::gstWeekDayName[(tsk.tipNext.rt.nDays+6)%7]);
                break;
            case RMD_TT_PER_MONTH:
                sBuf.Format(CPubData::gstRmdTimeTypeName[nType], 
                    tsk.tipNext.rt.nDays);
                break;
            case RMD_TT_DAYS:
                sBuf.Format(CPubData::gstRmdTimeTypeName[nType], 
                    tsk.tipNext.rt.nDays);
                break;
            case RMD_TT_EXACT_TIME:
                sBuf.Format(CPubData::gstRmdTimeTypeName[nType]);
                break;
            default:
                sBuf = "";
                break;
            }
            
            break;
        }
    case 5:
        {
            CTime ti(tsk.tiStartSet);
            sBuf = "";
            if (ti != 0)
            {
                sBuf = ti.Format("%H:%M:00");
            }
            break;
        }
    case 6:
        {
            sBuf = tsk.stDetails;
            break;
        }
    case 7:
        {
            if (CheckMsgTypeRemind(tsk.nMsgType))
            {
                sBuf = "是";
            }
            else
            {
                sBuf = "否";
            }
            break;
        }
    case 8:
        {
            sBuf = tsk.stCmd;
            break;
        }
    default:
        {
            sBuf = "";
            break;
        }
    }

    return sBuf;
}

time_t GetTimeFromString( const char* stTime )
{
    struct tm ti;
    memset(&ti, 0, sizeof(ti));

    if (sscanf(stTime, "%4d-%2d-%2d %2d:%2d", &ti.tm_year, 
        &ti.tm_mon, &ti.tm_mday, &ti.tm_hour, &ti.tm_min) == 5)
    {
        ti.tm_year -= 1900;
        ti.tm_mon -= 1;
        return mktime(&ti);
    }
    return 0;
}

short GetWeekFromString( const char* st ) 
{
    for (int i=0; i<7; ++i)
    {
        if (strcmp(st, CPubData::gstWeekDayName[i]) == 0)
        {
            return i + 1;
        }
    }
    return 0;
}

void GetRmdTypeFromString(TaskMsg& tsk, const char* st)
{
    for (int i=0; i<RMD_TT_MAX; ++i)
    {
        if (strncmp(st, CPubData::gstRmdTimeTypeName[i], 4) == 0)
        {
            tsk.tipNext.rt.nTypeTime = i;
            if (i == RMD_TT_PER_DAY || i == RMD_TT_EXACT_TIME)
            {
                tsk.tipNext.rt.nDays = 1;
            }
            else if (i == RMD_TT_PER_WEEK)
            {
                tsk.tipNext.rt.nDays = GetWeekFromString(st + 4);
            }
            else
            {
                sscanf(st, CPubData::gstRmdTimeTypeName[i], &(tsk.tipNext.rt.nDays));
            }
        }
    }
}

TConfig::TConfigImpl::TConfigImpl()
{
    LogLevel = 0;
    IsTaskTitleModifiable = false;
    WeatherShow = 0;
}

TConfig::TConfigImpl::~TConfigImpl()
{

}
