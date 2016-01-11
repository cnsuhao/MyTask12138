// 注册启动项.h : main header file for the 注册启动项 application
//

#if !defined(AFX__H__1AE9DE3F_A59B_4039_B4F2_C47A23C9DD81__INCLUDED_)
#define AFX__H__1AE9DE3F_A59B_4039_B4F2_C47A23C9DD81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
 
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegStartUpApp:
// See 注册启动项.cpp for the implementation of this class
//

class CRegStartUpApp : public CWinApp
{
public:
	CRegStartUpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegStartUpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegStartUpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__H__1AE9DE3F_A59B_4039_B4F2_C47A23C9DD81__INCLUDED_)
