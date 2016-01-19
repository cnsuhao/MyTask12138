// 我的任务Dlg.h : header file
//

#if !defined(AFX_DLG_H__F187D611_A2BC_4A54_914C_83D59D9FDE10__INCLUDED_)
#define AFX_DLG_H__F187D611_A2BC_4A54_914C_83D59D9FDE10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0501
#define _WIN32_IE 0x0501
#include <SHELLAPI.H>
#include <afxtempl.h>
#include "Types.h"
#include "MyButton.h"

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class ColorNode
{
public:
	int nPos;
	bool bIsOdd;
	ColorNode()
	{
		nPos = 0;
		bIsOdd = true;
	}
};

class CMyDlg : public CDialog
{
// Construction
public:
	bool RedrawListCtrl();
	bool RedrawListCtrl(int item);
	CMyDlg(CWnd* pParent = NULL);	// standard constructor
	void OnOK();
	void OnCancel();

	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);

// 数据相关方法
public:
	void ClearTimeShaft();
	virtual  ~CMyDlg();
	void OnRealExit();
	int RebuildDate();
	CString GetDate(time_t ti);
	void SetDataToListCtrl(int viewIndex, bool IsAdd);
	bool LoadFileHead(FILE*fp, FileHead& fileHead);
	bool LoadFileRecord(FILE*fp, tagTaskMsg& task);
	bool WriteFileHead(FILE*fp, const FileHead& fileHead);
	bool WriteFileRecord(FILE*fp, const TaskMsg& task);
    PTaskMsg GetTimeoutTaskAt(int index_)
    {
        if (index_ >=0 && index_ < GetTimeoutTaskCount())
        {
            return m_arrTimeoutTasks.ElementAt(index_);
        }
        return NULL;
    }
    int GetTimeoutTaskCount()
    {
        return m_arrTimeoutTasks.GetCount();
    }

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	CComboBox	m_ctlSelectDate;
	CListCtrl	m_ctlListTmp;
	CListCtrl	m_ctlList;
	CMyButton	m_btnExit;
	CMyButton	m_btnHide;
	CString	m_stStatus;
	int		m_nSelectDate;
	CString	m_stFileType;
	CString	m_stCurTime;
	CString	m_stShowDate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CDialog* m_pEditDlg;
	HICON m_hIcon;
	int m_nIconIndex;
	NOTIFYICONDATA m_tnid;
	int m_nTipIndex, m_nTipType;
	CToolBar m_ctlToolBar;
	// 所有普通任务
	CArray<TaskMsg, TaskMsg&> m_arrTasks;
	//CArray<TaskMsg, TaskMsg&> m_arrDfTasks;
	CArray<int, int&> m_arrDateList;
	// 下标是视图中的下标；值是在Task列表中的下标
	CArray<int, int&> m_arrIndexMap;
	CArray<ColorNode, ColorNode&> m_arrTimeNode;
	CArray<TaskMsg, TaskMsg&> m_arrCpy;
    CArray<PTaskMsg, PTaskMsg> m_arrTimeoutTasks;
	int m_nColorPt[720];
	CRect m_objRect;
	CMap<int, int, CString, LPCTSTR> m_mapTip;
	CBitmap m_myBitmap;
    int m_iSortIndex;
    bool m_bIsAscending;
    enum TagSortFlag
    {
        SF_NO_FLAG = 0,
        SF_ASC_FLAG,
        SF_DESC_FLAG
    };
    bool m_bIsOnStart;  // 是否在程序刚开始时

protected:
	bool OnExportAllByDay();
	bool OnExportAll();
	CString m_stOpenLogCmd;
	bool WriteOperatorLog(const char* );
	CString m_stLastStatus;
	bool DoRemind(TaskMsg& task, int & type, int & mins);
	void SetChange(bool bIsChanged);
	bool m_bIsChanged;
	FileHead m_objFileHead;
	void OnNotDefined();
	void OnMyShowWindow();
	CString m_stCurFileName;
	int m_nLastDateSel;
	void ChangeView( bool isBak );
	void CheckAll();
	bool DoOpen(const CString& stFileName);
	bool DoOpenDfTasks(const CString& stFileName);
	bool DoSave(const CString& stFileName );
	bool DoSaveDfTasks(const CString& stFileName );
	void RebuildTimeShaft();
	void SetTips();
	void CheckTaskRemind();

    bool DoTaskCommandImpl();

    BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
	void InitSomeTasks();
	void GetCellName( int p1, int p2, CString& colname );
	bool SetTaskToExcel( int iTaskIndex, int iSheetIndex, COleSafeArray& saRet);

	void ModifyTime();
	bool CheckShow( const TaskMsg& task, const CString stDate);
	void DoRedrawTrayIcon(bool bReset=false);
	LRESULT OnTrayIconTipOff( WPARAM wParam,LPARAM lParam );

	void OnMenuCopyTask();
	void OnMenuClearCopy();
	void OnMenuPasteTask();

    void OnStart(); // 程序刚开始的批量处理

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonToolAdd();
	afx_msg void OnButtonToolDel();
	afx_msg void OnMenuDel();
	afx_msg void OnButtonToolEdit();
	afx_msg void OnButtonToolOpen();
	afx_msg void OnButtonToolOpenDefault();
	afx_msg void OnButtonToolSave();
	afx_msg void OnButtonToolSaveDefault();
	afx_msg void OnButtonToolSaveAs();
	afx_msg void OnButtonToolSet();
	afx_msg void OnButtonToolSub();
	afx_msg void OnButtonToolFinish();
	afx_msg void OnButtonToolExit();
	afx_msg void OnButtonToolExport();
	afx_msg void OnClickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListTaskShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboSelect();
	afx_msg LRESULT OnNotifyTray(WPARAM wParam,LPARAM lParam); // 托盘图标消息
	afx_msg LRESULT OnMsgShow(WPARAM wParam,LPARAM lParam); 
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateMainSubmit(CCmdUI* pCmdUI);
	afx_msg BOOL OnToolTipText( UINT NID, NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMainViewLog();
	afx_msg void OnExportCur();
	afx_msg void OnMainSetViewTool();
	afx_msg void OnDblclkListTaskShow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStaticMyIcon();
	afx_msg void OnMainDfTasks();
    virtual void WinHelp( DWORD_PTR dwData, UINT nCmd = HELP_CONTEXT );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void PrePasteTask( TaskMsg& tsk );
	void OnExportCurTxt();
    void OnExportAllTxt();
    void OnExportAllByDayTxt();
    bool IfRebuildTimeShaft(int index_)
    {
        if (index_ == SI_ALL || index_ == SI_NON_FINISHED)
        {
            return false;
        }
        return true;
    }
    void CheckDfTaskRemind();
    bool DoTaskCommand( const TaskMsg& tsk );
    bool SortBySubItemAndRedraw( int iSubItem );
    void SetColumnSortFlag( int m_iSortIndex, TagSortFlag flag );
public:
    afx_msg void OnAbout();
public:
    afx_msg void OnMenuMainIniEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__F187D611_A2BC_4A54_914C_83D59D9FDE10__INCLUDED_)
