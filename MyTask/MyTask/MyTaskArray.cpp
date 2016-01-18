// MyTaskArray.cpp: implementation of the MyTaskArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTask.h"
#include "MyTaskArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyTaskArray::MyTaskArray()
{
	m_pTasks = NULL;
	m_nMaxCount = 0;
	m_nCount = 0;
	m_bIsChanged = false;
}

MyTaskArray::~MyTaskArray()
{

}

void MyTaskArray::AddTask(TaskMsg& msg )
{
	if (m_nCount >= m_nMaxCount)
	{
		if (NULL == m_pTasks)
		{
			m_pTasks = new TaskMsg[DEFAULT_COUNT];
            m_nCount = 0;
            m_nMaxCount = DEFAULT_COUNT;
		}
		else
		{
			PTaskMsg p = new TaskMsg[m_nMaxCount + COUNT_INC];
			for(int i=0; i<m_nMaxCount; i++)
			{
				p[i] = m_pTasks[i];
			}
			delete [] m_pTasks;
			
			m_pTasks = p;
			m_nMaxCount += COUNT_INC;
		}
	}
	
	m_pTasks[m_nCount] = msg;
	m_nCount++;
	m_bIsChanged = true;
	return;
}

int MyTaskArray::GetCount()
{
	return m_nCount;
}

TaskMsg& MyTaskArray::operator[]( int index )
{
	return GetTaskAt(index);
}

TaskMsg& MyTaskArray::GetTaskAt( int index )
{
	static TaskMsg staTaskMsg;
	if (index >= m_nCount)
	{
		return staTaskMsg;
	}
	return m_pTasks[index];
}

int MyTaskArray::DelMsgAt( int index )
{
	if (index >= m_nCount)
	{
		return 0;
	}
	for(int i= index+1; i<m_nCount; i++)
	{
		m_pTasks[i-1] = m_pTasks[i];
	}
	m_nCount--;
	m_bIsChanged = true;
	return m_nCount;
}

bool MyTaskArray::IsChanged()
{
	return m_bIsChanged;
}

void MyTaskArray::SetChange( bool bChange /* =true */ )
{
	m_bIsChanged = bChange;
}

PTaskMsg MyTaskArray::GetHeadPointer()
{
	return m_pTasks;
}

int MyTaskArray::DelAll()
{
	m_nCount = 0;
	return m_nCount;
}

