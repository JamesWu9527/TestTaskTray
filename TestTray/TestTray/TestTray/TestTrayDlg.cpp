
// TestTrayDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TestTray.h"
#include "TestTrayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestTrayDlg 对话框

#define WM_TASKTRAY (WM_USER + 1)

CTestTrayDlg::CTestTrayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTTRAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestTrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestTrayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_TASKTRAY,OnTaskTray)
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CTestTrayDlg 消息处理程序

BOOL CTestTrayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestTrayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestTrayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTestTrayDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

//参数说明：①接收图标ID ②接收鼠标的行为
LRESULT CTestTrayDlg::OnTaskTray(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
		case WM_RBUTTONUP:	//捕获右键弹起 事件  设置一个菜单
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);	//得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();	//弹出菜单
			menu.AppendMenuW(MF_STRING, WM_DESTROY, L"关闭");
			SetForegroundWindow();	//将创建窗口的线程放入前台并激活窗口
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
			
			//释放菜单资源
			menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
		case WM_LBUTTONDBLCLK:	//双击左键的处理
		{
			ShowWindow(SW_SHOWNORMAL);	//显示窗口
			DelTaskTrayIcon();	//删除托盘部分代码
		}
		break;
	}
	return LRESULT();
}

//增加托盘图标
void CTestTrayDlg::AddTaskTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_TASKTRAY;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	StrCpyW(nid.szTip, L"TestTray");
	Shell_NotifyIcon(NIM_ADD, &nid);	
}

void CTestTrayDlg::DelTaskTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	//nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//nid.uCallbackMessage = WM_TASKTRAY;
	//nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//StrCpyW(nid.szTip, L"TestTray");
	Shell_NotifyIcon(NIM_DELETE, &nid);	//在托盘删除图标
}

void CTestTrayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)	//捕获到最小化的命令
	{
		ShowWindow(SW_HIDE);
		AddTaskTrayIcon();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CTestTrayDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//删除托盘图标
	DelTaskTrayIcon();
}


void CTestTrayDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScreenToClient(&point);

	//LPPOINT lpoint = new tagPOINT;
	//::GetCursorPos(lpoint);	//得到鼠标位置
	CMenu menu;
	menu.CreatePopupMenu();	//弹出菜单
	menu.AppendMenuW(MF_STRING, WM_DESTROY, L"关闭");
	SetForegroundWindow();	//将创建窗口的线程放入前台并激活窗口
	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	//menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);

	//释放菜单资源
	menu.Detach();
	menu.DestroyMenu();
	//delete lpoint;
	CDialogEx::OnRButtonUp(nFlags, point);
}
