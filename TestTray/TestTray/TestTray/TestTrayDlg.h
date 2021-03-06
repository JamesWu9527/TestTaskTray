
// TestTrayDlg.h: 头文件
//

#pragma once


// CTestTrayDlg 对话框
class CTestTrayDlg : public CDialogEx
{
// 构造
public:
	CTestTrayDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTTRAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	//预处理，在界面初始胡化之前处理的一些操作
	LRESULT OnTaskTray(WPARAM wParam, LPARAM lParam);	//和托盘相关的事件
	void AddTaskTrayIcon();		//添加托盘图标
	void DelTaskTrayIcon();		//删除托盘图标
	afx_msg void OnDestroy();	//销毁程序
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	//右键点击消息
};
