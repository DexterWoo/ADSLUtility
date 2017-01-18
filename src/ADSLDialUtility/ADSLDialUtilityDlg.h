
// ADSLDialUtilityDlg.h : ͷ�ļ�
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
// CADSLDialUtilityDlg �Ի���
class CADSLDialUtilityDlg : public CDialogEx
{
// ����
public:
	VOID UpdateStates(RASCONNSTATE rasconnstate, DWORD dwError);

	void AppendIP(CString strText);

	void Redial();

	CString GetStateDescription(RASCONNSTATE rasconnstate, CString strText);

	CADSLDialUtilityDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ADSLDIALUTILITY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	void InitUI();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��������
	CButton m_chkBookRun;
	// �Զ�ɾ��Cookies
	CButton m_chkCookies;
	/*// �����ȼ�
	CButton m_chkHotKey;*/
	// ɾ��IE��ʱ�ļ�
	CButton m_chkIETemp;
	// IP�ظ����
	CButton m_chkIPRepeat;
	// �Զ���������
	CButton m_chkTimeConnect;
	// ��������
	CComboBox m_cmbConnection;
/*
	// �ȼ�ѡ��
	CComboBox m_cmbHotkey;*/
	// ��ʾ��Ϣ
//	CStatic m_stStatus;
	// IP�б�
	CListBox m_lstIP;
	// ״̬��ʾ��Ϣ
	CLabel m_stStatus;
	// ��ǰIP��ַ
	CLabel m_stCurIP;
	// ����ʱ�����
	unsigned long m_ulConnectionTime;
	// ɾ��Cookiesʱ��
	unsigned long m_ulCookies;
	// �Ͽ�ʱ��
	unsigned long m_ulHangupTime;
	// ɾ��IE��ʱ�ļ�
	unsigned long m_ulIETmp;
	// �û���
	CString m_strUserName;
	// ���루���ģ�
	CString m_strPwd;
	
	//���ſ�����
	CRasControl m_rascontrol;
	//IE������
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
	// ����ʱ���༭��
	CEditEx m_edtConnectTime;
	// Cookies �༭��
	CEditEx m_edtCookies;
	// �Ͽ�ʱ���༭��
	CEditEx m_edtHanguptime;
	// ���IE����༭��
	CEditEx m_edtIETmp;
	// �û���
	CEditEx m_edtUsername;
	// �����
	CEditEx m_edtPwd;
	//״̬���Ƿ�Ҫȥ�ж�IP�ظ�
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
