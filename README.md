# TestTaskTray
实现软件点击最下化 在桌面托盘处显示出来

最小化的原理：首先要将窗口隐藏，然后 在右下角绘制图标
恢复的原理：将窗口显示，再将托盘中的图片删除

主要遇到了三个问题，这里记录下
1.点击最小化，托盘确实产生了，但是桌面下方的状态栏还是会有大图标需要处理
解决方案
产生原因：在OnSysCommand函数中处理最小化命令的时候，捕获到最小化命令之后不在调用CDialogEx::OnSysCommand(nID, lParam);
	使用else区分开来
	if (nID == SC_MINIMIZE)	//捕获到最小化的命令
	{
		AddTaskTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
	如果继续调用CDialogEx::OnSysCommand(nID, lParam); 就会出现任务栏还是有个图标存在
  
2.最小化菜单上右键产生菜单 如果不点，鼠标移到别的地方点击 菜单还是不会消失，需要处理
在TrackPopupMenu之前使用函数SetForegroundWindow()可以解决此类问题
函数SetForegroundWindow();	//将创建窗口的线程放入前台并激活窗口

3.在点击关闭后，托盘未能及时消失
在OnDestroy函数中添加删除托盘图标的代码
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	//nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//nid.uCallbackMessage = WM_TASKTRAY;
	//nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//StrCpyW(nid.szTip, L"TestTray");
	Shell_NotifyIcon(NIM_DELETE, &nid);
