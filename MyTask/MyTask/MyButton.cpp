// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SelectClipRgn(&m_rgn);
	CRect rect=lpDrawItemStruct->rcItem;//得到按键区域
	CString sCaption;
	VERIFY(lpDrawItemStruct->CtlType==ODT_BUTTON);
	GetWindowText(sCaption);//得到按键的标题
	CBitmapButton::SetWindowRgn(m_rgn, FALSE);
	CBitmapButton::DrawItem(lpDrawItemStruct);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sCaption,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectClipRgn(NULL);
}

void CMyButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
// 
// 	ModifyStyle(0, BS_OWNERDRAW);
// 	CRect rect;
// 	GetClientRect(rect);
// 	m_rgn.DeleteObject();
// 	SetWindowRgn(NULL, FALSE);
// 	m_rgn.CreateEllipticRgnIndirect(rect);
// 	//m_rgn.CreateEllipticRgn(0, 0, 200, 100);
// 	SetWindowRgn(m_rgn, TRUE);
// 	
// 	// Convert client coords to the parents client coords
// 	ClientToScreen(rect);
// 	CWnd* pParent = GetParent();
// 	if (pParent) pParent->ScreenToClient(rect);
// 	
// 	// Resize the window
// 	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
// 	// Resize the window to make it square
// 	
// 	// Get the vital statistics of the window
// // 	rect.left+=5;
// // 	rect.right -=5;
// // 	rect.top+=3;
// // 	rect.bottom -=3;
}
