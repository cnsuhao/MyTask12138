// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6080E494_16D7_4E89_9859_F60218758C5D__INCLUDED_)
#define AFX_STDAFX_H__6080E494_16D7_4E89_9859_F60218758C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0501
#define _WIN32_IE 0x0501

#ifndef WINVER        // 指定要求的最低平台是 Windows Vista。
#define WINVER 0x0501    // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT    // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0501  // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif            


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#pragma warning(disable: 4996)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6080E494_16D7_4E89_9859_F60218758C5D__INCLUDED_)
