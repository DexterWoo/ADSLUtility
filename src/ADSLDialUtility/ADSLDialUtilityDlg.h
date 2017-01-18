
// ADSLDialUtilityDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Label.h"
#include "EditEx.h"
#include "rascontrol.h"
#include "AppConfig.h"
#include "IEControl.h"
#include "trayicon.h"
#include "RemoteController.h"
// CADSLDialUtilityDlg 对话框
class CADSLDialUtilityDlg : public CDialogEx
{
// 构造
public:
	VOID UpdateStates(RASCONNSTATE rasconnstate, DWORD dwError);

	void AppendIP(CString strText);

	void Redial();

	CString GetStateDescription(RASCONNSTATE rasconnstate, CString strText);

	CADSLDialUtilityDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ADSLDIALUTILITY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void InitUI();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 开机自启
	CButton m_chkBookRun;
	// 自动删除Cookies
	CButton m_chkCookies;
	/*// 启用热键
	CButton m_chkHotKey;*/
	// 删除IE临时文件
	CButton m_chkIETemp;
	// IP重复检测
	CButton m_chkIPRepeat;
	// 自动断线重连
	CButton m_chkTimeConnect;
	// 连接名称
	CComboBox m_cmbConnection;
/*
	// 热键选择
	CComboBox m_cmbHotkey;*/
	// 提示信息
//	CStatic m_stStatus;
	// IP列表
	CListBox m_lstIP;
	// 状态提示信息
	CLabel m_stStatus;
	// 当前IP地址
	CLabel m_stCurIP;
	// 连接时间参数
	unsigned long m_ulConnectionTime;
	// 删除Cookies时间
	unsigned long m_ulCookies;
	// 断开时间
	unsigned long m_ulHangupTime;
	// 删除IE临时文件
	unsigned long m_ulIETmp;
	// 用户名
	CString m_strUserName;
	// 密码（明文）
	CString m_strPwd;
	
	//拨号控制类
	CRasControl m_rascontrol;
	//IE控制类
	CIEControl m_IEControl;

	CTrayIcon Tray;

	afx_msg void OnBnClickedCheckCookies();
	afx_msg void OnBnClickedCheckIetemp();
	afx_msg void OnBnClickedCheckTimeconnect();
	afx_msg void OnBnClickedCheckBootrun();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonHangup();
	afx_msg void OnBnClickedButtonSavesettings();
	afx_msg void OnBnClickedCheckIprepeat();
	afx_msg void OnBnClickedCheckHotkey();
	afx_msg void OnSelchangeComboConnection();

	void SwitchConnection(int iCurSel);

	afx_msg void OnSelchangeComboHotkey();
	// 连接时长编辑框
	CEditEx m_edtConnectTime;
	// Cookies 编辑框
	CEditEx m_edtCookies;
	// 断开时长编辑框
	CEditEx m_edtHanguptime;
	// 清空IE缓存编辑框
	CEditEx m_edtIETmp;
	// 用户名
	CEditEx m_edtUsername;
	// 密码框
	CEditEx m_edtPwd;
	//状态，是否要去判断IP重复
	BOOL m_bIPRepertCheck;
	CString m_strFileName;
	CAppConfig m_appConfig;
	CStringList m_IPList;
	CRemoteController m_RemoteController;
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShow();
	afx_msg void OnExit();
	afx_msg void OnAfxIdsScminimize();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnUpdateAfxIdsScminimize(CCmdUI *pCmdUI);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnRedial(WPARAM wParam, LPARAM lParam);
};
