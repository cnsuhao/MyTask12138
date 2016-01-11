// MyTaskArray.h: interface for the MyTaskArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTASKARRAY_H__A3F6FDD8_FA02_4725_9850_79414A16D325__INCLUDED_)
#define AFX_MYTASKARRAY_H__A3F6FDD8_FA02_4725_9850_79414A16D325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Types.h"

class MyTaskArray  
{
public:
#define DEFAULT_COUNT 10
#define COUNT_INC  5
public:
	MyTaskArray();
	~MyTaskArray();

public:
	int GetCount();
	TaskMsg& GetTaskAt(int index);
	TaskMsg& operator[](int index);
	void AddTask( TaskMsg& msg );
	int DelMsgAt(int index);
	int DelAll();
	bool IsChanged();
	void SetChange(bool bChange = true);
	PTaskMsg GetHeadPointer();

private:
	bool m_bIsChanged;
	int m_nCount;
	int m_nMaxCount;
	PTaskMsg m_pTasks;
};

#endif // !defined(AFX_MYTASKARRAY_H__A3F6FDD8_FA02_4725_9850_79414A16D325__INCLUDED_)
