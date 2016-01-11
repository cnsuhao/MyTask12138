========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : 我的任务
========================================================================


AppWizard has created this 我的任务 application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your 我的任务 application.

我的任务.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

我的任务.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CMyApp application class.

我的任务.cpp
    This is the main application source file that contains the application
    class CMyApp.

我的任务.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

我的任务.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\我的任务.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file 我的任务.rc.

res\我的任务.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.




/////////////////////////////////////////////////////////////////////////////

AppWizard creates one dialog class:

我的任务Dlg.h, 我的任务Dlg.cpp - the dialog
    These files contain your CMyDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's
    template is in 我的任务.rc, which can be edited in Microsoft
	Visual C++.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named 我的任务.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
版本说明：

v1.0		最初版本
				① 界面绘制
				② 窗口布局
				③ 消息格式，加解密操作，文件操作

v1.1		功能版本
				① 任务增删改操作，任务提醒
				② 配置窗口与配置存取
				③ 工具栏、菜单与托盘图标实现
				④ 添加任务按天显示功能

v1.2		功能强化与美化版本
				① 音乐管理
				② 工具栏美化（添加文字）
				③ 按钮美化
				④ 添加按日的时间段显示
				⑤ 添加背景图片

v1.3		进一步强化版本（2012-09-26）
				① 自定义背景图片功能；
				② 添加操作日志功能（2012-09-25）和日志查看功能（2012-09-26）；
				③ 添加菜单和列表的背景图片功能（2012-09-25）；

v1.4		设置添加预留新版本（开发中...）【这样不用因为添加设置就添加新版本了】
				① 添加开机自动启动功能（2012-09-27）；
				② 添加设置文件预留功能（暂时预留256字节，待扩展）。

/////////////////////////////////////////////////////////////////////////////
