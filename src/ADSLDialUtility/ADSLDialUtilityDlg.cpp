
// ADSLDialUtilityDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADSLDialUtility.h"
#include "ADSLDialUtilityDlg.h"
#include "afxdialogex.h"
#include "RemoteTaskDefs.h"
#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

#define WM_ICON_NOTIFY		(WM_USER + 205)
#define  EmptyString _T("")

#define	TIMER_COOKIE	0x1989
#define	TIMER_IE		0x1990
#define	TIMER_CON		0x1991
#define TIMER_GETIP		0x1992
#define TIMER_IPREPET	0x1993
// CADSLDialUtilityDlg �Ի���
VOID CADSLDialUtilityDlg::UpdateStates(RASCONNSTATE rasconnstate, DWORD dwError)
{
	CString strText;

	strText = GetStateDescription(rasconnstate, strText);

	if (!m_cmbConnection.IsWindowEnabled() || RASCS_Disconnected == rasconnstate)
	{
		m_stCurIP.SetText(strText);
		Tray.SetTooltipText(strText);
	}
	if (RASCS_AuthNotify == rasconnstate && dwError != 0)
	{
		Redial();
	}
	
	if (rasconnstate == RASCS_Connected)
	{
		Sleep(500);
		strText = m_rascontrol.GetCurIP();
		if (strText != EmptyString)
		{
			if (m_IPList.Find(strText) == NULL)
			{
				AppendIP(strText);

			}
			else
			{
				//IP�ظ��ˣ�ȥ��
				if (m_appConfig.IPRepeat == BST_CHECKED)
				{
					Sleep(500);
					strText += STR((IP�ظ��ˣ����²���...));
					m_stCurIP.SetText(strText);
					Redial();
				}
			}
		}
		else
		{
			OnBnClickedButtonHangup();
			OnBnClickedButtonConnect();
		}
		//��ʱ����...
		if (m_appConfig.TimedConnect == BST_CHECKED)
		{
			SetTimer(TIMER_CON, m_appConfig.TimeConnection * 1000, NULL);
		}
		
		m_stCurIP.SetText(strText);
	}
}


void CADSLDialUtilityDlg::AppendIP(CString strText)
{
	if (m_lstIP.GetCount() >= 500)
	{
		//save iplist to file. and clean the GUI.
		TCHAR buffer[256];
		GetModuleFileName(NULL, buffer, 256);
		*_tcsrchr(buffer, '\\') = '\0';
		CString strFileName = buffer;
		CString timestr = CTime::GetCurrentTime().Format(STR(\\HistoryIPs\\%Y%m%d));
		CreateDirectory(strFileName + STR(\\HistoryIPs), NULL);
		CreateDirectory(strFileName + timestr, NULL);
		timestr = CTime::GetCurrentTime().Format(STR(\\HistoryIPs\\%Y%m%d\\%H%M%S.txt));

		CStdioFile file;
		file.Open(strFileName + timestr, CFile::modeCreate | CFile::modeWrite);
		
		while (!m_IPList.IsEmpty())
		{
			file.WriteString(m_IPList.RemoveHead() + STR(\n));
		}
		file.Close();
		m_lstIP.ResetContent();
		m_IPList.RemoveAll();
	}
	m_lstIP.AddString(strText);
	m_lstIP.SetCurSel(m_lstIP.GetCount() - 1);
	m_IPList.AddTail(strText);
}

void CADSLDialUtilityDlg::Redial()
{
	OnBnClickedButtonHangup();
	SetTimer(TIMER_IPREPET, m_appConfig.TimeHangup * 1000, NULL);
}

CString CADSLDialUtilityDlg::GetStateDescription(RASCONNSTATE rasconnstate, CString strText)
{
	switch (rasconnstate)
	{
	case RASCS_OpenPort:
		strText = _T("���ڴ򿪶˿�...");
		break;

	case RASCS_PortOpened:
		strText = _T("�˿��Ѿ���");
		break;

	case RASCS_ConnectDevice:
		strText = _T("���������豸");
		break;

	case RASCS_DeviceConnected:
		strText = _T("�豸�Ѿ�������");
		break;

	case RASCS_AllDevicesConnected:
		strText = _T("�����豸�Ѿ�������");
		break;

	case RASCS_Authenticate:
		strText = _T("������֤�û���������...");
		break;

	case RASCS_AuthNotify:
		strText = _T("����ʧ��.");
		break;

	case RASCS_AuthRetry:
		strText = _T("�ٴ���֤�û���������...");
		break;

	case RASCS_AuthCallback:
		strText = _T("������֤");
		break;

	case RASCS_AuthChangePassword:
		strText = _T("�ı���������");
		break;

	case RASCS_AuthProject:
		strText = _T("��ʼ����״̬");
		break;

	case RASCS_AuthLinkSpeed:
		strText = _T("���������ٶ�...");
		break;

	case RASCS_AuthAck:
		strText = _T("Authentication Acknowledged");
		break;

	case RASCS_ReAuthenticate:
		strText = _T("Reauthenticatation Started");
		break;

	case RASCS_Authenticated:
		strText = _T("Authenticated");
		break;

	case RASCS_PrepareForCallback:
		strText = _T("Preparation For Callback");
		break;

	case RASCS_WaitForModemReset:
		strText = _T("�ȴ��豸����...");
		break;

	case RASCS_WaitForCallback:
		strText = _T("Waiting For Callback");
		break;

	case RASCS_Interactive:
		strText = _T("Interactive");
		break;

	case RASCS_RetryAuthentication:
		strText = _T("�ٴ���֤");
		break;

	case RASCS_CallbackSetByCaller:
		strText = _T("Callback Set By Caller");
		break;

	case RASCS_PasswordExpired:
		strText = _T("�������");
		break;

	case RASCS_Connected:
		strText = _T("�Ѿ�������");
		break;

	case RASCS_Disconnected:
		strText = _T("�Ѿ��Ͽ�����");
		break;

	default:
		strText = _T("δ����Ĵ������");
		break;
	}	
	return strText;
}

CADSLDialUtilityDlg::CADSLDialUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADSLDialUtilityDlg::IDD, pParent)
	, m_ulConnectionTime(0)
	, m_ulCookies(0)
	, m_ulHangupTime(0)
	, m_ulIETmp(0)
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADSLDialUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_CHECK_BOOTRUN, m_chkBootRun);
	DDX_Control(pDX, IDC_CHECK_BOOTRUN, m_chkBookRun);
	DDX_Control(pDX, IDC_CHECK_COOKIES, m_chkCookies);
//	DDX_Control(pDX, IDC_CHECK_HOTKEY, m_chkHotKey);
	DDX_Control(pDX, IDC_CHECK_IETEMP, m_chkIETemp);
	DDX_Control(pDX, IDC_CHECK_IPREPEAT, m_chkIPRepeat);
	DDX_Control(pDX, IDC_CHECK_TIMECONNECT, m_chkTimeConnect);
	DDX_Control(pDX, IDC_COMBO_CONNECTION, m_cmbConnection);
//	DDX_Control(pDX, IDC_COMBO_HOTKEY, m_cmbHotkey);
	//  DDX_Control(pDX, IDC_STATIC_STATUS, m_stStatus);
	DDX_Control(pDX, IDC_LIST_IP, m_lstIP);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_stStatus);
	DDX_Control(pDX, IDC_STATIC_CURIP, m_stCurIP);
	DDX_Control(pDX, IDC_EDIT_CONNECTTIME, m_edtConnectTime);
	DDX_Control(pDX, IDC_EDIT_COOKIES, m_edtCookies);
	DDX_Control(pDX, IDC_EDIT_HANGUPTIME, m_edtHanguptime);
	DDX_Control(pDX, IDC_EDIT_IETEMP, m_edtIETmp);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_edtUsername);
	DDX_Control(pDX, IDC_EDIT_USERPWD, m_edtPwd);
}

BEGIN_MESSAGE_MAP(CADSLDialUtilityDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_COOKIES, &CADSLDialUtilityDlg::OnBnClickedCheckCookies)
	ON_BN_CLICKED(IDC_CHECK_IETEMP, &CADSLDialUtilityDlg::OnBnClickedCheckIetemp)
	ON_BN_CLICKED(IDC_CHECK_TIMECONNECT, &CADSLDialUtilityDlg::OnBnClickedCheckTimeconnect)
	ON_BN_CLICKED(IDC_CHECK_BOOTRUN, &CADSLDialUtilityDlg::OnBnClickedCheckBootrun)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CADSLDialUtilityDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_HANGUP, &CADSLDialUtilityDlg::OnBnClickedButtonHangup)
//	ON_BN_CLICKED(IDC_BUTTON_SAVESETTINGS, &CADSLDialUtilityDlg::OnBnClickedButtonSavesettings)
	ON_BN_CLICKED(IDC_CHECK_IPREPEAT, &CADSLDialUtilityDlg::OnBnClickedCheckIprepeat)
	//ON_BN_CLICKED(IDC_CHECK_HOTKEY, &CADSLDialUtilityDlg::OnBnClickedCheckHotkey)
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECTION, &CADSLDialUtilityDlg::OnSelchangeComboConnection)
	ON_CBN_SELCHANGE(IDC_COMBO_HOTKEY, &CADSLDialUtilityDlg::OnSelchangeComboHotkey)
	ON_WM_TIMER()
	ON_MESSAGE(WM_ICON_NOTIFY, &CADSLDialUtilityDlg::OnIconNotify)
	ON_COMMAND(IDM_SHOW, &CADSLDialUtilityDlg::OnShow)
	ON_COMMAND(IDM_EXIT, &CADSLDialUtilityDlg::OnExit)
	ON_COMMAND(AFX_IDS_SCMINIMIZE, &CADSLDialUtilityDlg::OnAfxIdsScminimize)
	ON_WM_SHOWWINDOW()
	ON_UPDATE_COMMAND_UI(AFX_IDS_SCMINIMIZE, &CADSLDialUtilityDlg::OnUpdateAfxIdsScminimize)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_REDIAL, &CADSLDialUtilityDlg::OnRedial)
END_MESSAGE_MAP()


// CADSLDialUtilityDlg ��Ϣ�������

BOOL CADSLDialUtilityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	TCHAR buffer[256];
	GetModuleFileName(NULL, buffer, 256);
	m_strFileName = buffer;

	InitUI();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CADSLDialUtilityDlg::InitUI()
{
	Tray.Create(this, WM_ICON_NOTIFY, _T("ADSL��������"),
		(HICON)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0),
		IDR_TRAY_MENU);
	GetDlgItem(IDC_BUTTON_HANGUP)->EnableWindow(FALSE);
	m_stCurIP.SetText(L"δ����");
	m_stStatus.SetText(EmptyString);
	UINT nEcntryCnt = m_rascontrol.GetEntryCount();
	CRasControl::pWnd = this;
	if (nEcntryCnt <= 0)
	{
		AfxMessageBox(_T("���ȴ����������"));
		exit(-1);
	}
	m_cmbConnection.ResetContent();
	for (UINT i = 0; i < nEcntryCnt; i++)
	{
		m_cmbConnection.AddString(m_rascontrol.GetEntryName(i));
	}
	if (m_appConfig.ConnectionIndex >= nEcntryCnt)
	{
		m_appConfig.ConnectionIndex = 0;
	}
	m_cmbConnection.SetCurSel(m_appConfig.ConnectionIndex);
	//SwitchConnection(m_appConfig.ConnectionIndex);
	m_edtCookies.SetWindowText(Int2CString(m_appConfig.TimeToDeleteCookies)); 
	m_edtIETmp.SetWindowText(Int2CString(m_appConfig.TimeToDeleteIETmp));
	m_edtConnectTime.SetWindowText(Int2CString(m_appConfig.TimeConnection));
	m_edtHanguptime.SetWindowText(Int2CString(m_appConfig.TimeHangup));

	m_chkBookRun.SetCheck(m_appConfig.BootStart);
	m_chkCookies.SetCheck(m_appConfig.DeleteCookies);
	m_chkIETemp.SetCheck(m_appConfig.DeleteIETmp);
	m_chkIPRepeat.SetCheck(m_appConfig.IPRepeat);
	m_chkTimeConnect.SetCheck(m_appConfig.TimedConnect);
	OnBnClickedCheckCookies();
	OnBnClickedCheckIetemp();
	OnBnClickedCheckIprepeat();
	OnBnClickedCheckTimeconnect();
	OnBnClickedCheckBootrun();
	OnSelchangeComboConnection();
	//����
	OnBnClickedButtonConnect();
	OnBnClickedButtonHangup();
	OnBnClickedButtonConnect();
	m_RemoteController.StartBGWork();
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CADSLDialUtilityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CADSLDialUtilityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CADSLDialUtilityDlg::OnBnClickedCheckCookies()
{
	m_appConfig.DeleteCookies = m_chkCookies.GetCheck();
	if (m_appConfig.DeleteCookies == BST_CHECKED)
	{
		m_appConfig.TimeToDeleteCookies = m_edtCookies.GetInt();
		if (m_appConfig.TimeToDeleteCookies == 0)
		{
			m_edtCookies.SetWindowText(L"600");
			m_appConfig.TimeToDeleteCookies = 600;
		}
		m_edtCookies.EnableWindow(FALSE);
		SetTimer(TIMER_COOKIE, m_appConfig.TimeToDeleteCookies * 1000, NULL);
	}
	else if (m_appConfig.DeleteCookies == BST_UNCHECKED)
	{
		m_edtCookies.EnableWindow(TRUE);
	}
}


void CADSLDialUtilityDlg::OnBnClickedCheckIetemp()
{
	m_appConfig.DeleteIETmp = m_chkIETemp.GetCheck();
	if (m_appConfig.DeleteIETmp == BST_CHECKED)
	{
		m_appConfig.TimeToDeleteIETmp = m_edtIETmp.GetInt();
		if (m_appConfig.TimeToDeleteIETmp == 0)
		{
			m_edtIETmp.SetWindowText(L"1800");
			m_appConfig.TimeToDeleteIETmp = 1800;
		}
		m_edtIETmp.EnableWindow(FALSE);
		SetTimer(TIMER_IE, m_appConfig.TimeToDeleteIETmp * 1000, NULL);
	}
	else if (m_appConfig.DeleteIETmp == BST_UNCHECKED)
	{
		m_edtIETmp.EnableWindow(TRUE);
		KillTimer(TIMER_IE);
	}
}


void CADSLDialUtilityDlg::OnBnClickedCheckTimeconnect()
{
	m_appConfig.TimedConnect = m_chkTimeConnect.GetCheck();
	if (m_appConfig.TimedConnect == BST_CHECKED)
	{
		m_appConfig.TimeConnection = m_edtConnectTime.GetInt();
		if (m_appConfig.TimeConnection == 0)
		{
			m_edtConnectTime.SetWindowText(L"30");
			m_appConfig.TimeConnection = 30;
		}
		m_appConfig.TimeHangup = m_edtHanguptime.GetInt();
		if (m_ulHangupTime == 0)
		{
			m_edtHanguptime.SetWindowText(L"7");
			m_appConfig.TimeHangup = 7;
		}
		m_edtConnectTime.EnableWindow(FALSE);
		m_edtHanguptime.EnableWindow(FALSE);
	}
	else if (m_appConfig.TimedConnect == BST_UNCHECKED)
	{
		m_edtConnectTime.EnableWindow(TRUE);
		m_edtHanguptime.EnableWindow(TRUE);
	}
}


void CADSLDialUtilityDlg::OnBnClickedCheckBootrun()
{
	m_appConfig.BootStart = m_chkBookRun.GetCheck();
	//���ÿ�������
	//��ע���
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		STR(Software\\Microsoft\\Windows\\CurrentVersion\\Run), 0, KEY_SET_VALUE, &hKey);
	if (m_appConfig.BootStart == BST_CHECKED)
	{
		//д��ע��� 
		RegSetValueEx(hKey, _T("{9E33576B-FE9A-4FCC-ADC2-38DE64A60EC4}"), 0, REG_SZ, (BYTE*)(LPCTSTR)m_strFileName, m_strFileName.GetLength()*2 + 2);
	}
	else if (m_appConfig.BootStart == BST_UNCHECKED)
	{
		//ɾ����ֵ
		RegDeleteValue(hKey, _T("{9E33576B-FE9A-4FCC-ADC2-38DE64A60EC4}"));
	}
	//�ر�ע���   
	RegCloseKey(hKey);
}


void CADSLDialUtilityDlg::OnBnClickedButtonConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ���ӣ��������漸���ؼ���
	m_cmbConnection.EnableWindow(FALSE);
	m_edtUsername.EnableWindow(FALSE);
	m_edtPwd.EnableWindow(FALSE);

	m_rascontrol.Dial(m_cmbConnection.GetCurSel());
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HANGUP)->EnableWindow(TRUE);
}


void CADSLDialUtilityDlg::OnBnClickedButtonHangup()
{
	//�Ͽ�������ؼ�������״̬��
	m_stCurIP.SetText(L"���ڶϿ�����...");
	m_rascontrol.HangUp(m_cmbConnection.GetCurSel());
	m_cmbConnection.EnableWindow(TRUE);
	m_edtUsername.EnableWindow(TRUE);
	m_edtPwd.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_HANGUP)->EnableWindow(FALSE);
	Sleep(500);
	UpdateStates(RASCS_Disconnected, 0);
}


void CADSLDialUtilityDlg::OnBnClickedButtonSavesettings()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���浽ini֮����ļ���

}


void CADSLDialUtilityDlg::OnBnClickedCheckIprepeat()
{
	m_appConfig.IPRepeat = m_chkIPRepeat.GetCheck();
}


void CADSLDialUtilityDlg::OnSelchangeComboConnection()
{
	// ѡ�����л��ˣ��л��û���������
	m_appConfig.ConnectionIndex = m_cmbConnection.GetCurSel();
	SwitchConnection(m_appConfig.ConnectionIndex);
}


void CADSLDialUtilityDlg::SwitchConnection(int iCurSel)
{
	m_edtUsername.SetWindowText(m_rascontrol.GetEntryUserName(iCurSel));
	m_edtPwd.SetWindowText(m_rascontrol.GetEntryPassword(iCurSel));
}

void CADSLDialUtilityDlg::OnSelchangeComboHotkey()
{
	//����ʲôҲ���,������

}

LRESULT CADSLDialUtilityDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO:  �ڴ����ר�ô����/����û���
	switch (message)
	{
	case WM_DIALSTATE_CHANGED:
	case  WM_RASDIALEVENT:
	{
		UpdateStates(RASCONNSTATE(wParam), lParam);
	}
	default:
		break;
	}
	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

void CADSLDialUtilityDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent)
	{
	case TIMER_IPREPET:
		OnBnClickedButtonConnect();
		break;
	case TIMER_IE:
		m_IEControl.ClearTmpFiles();
		if (m_appConfig.DeleteIETmp)
		{
			SetTimer(TIMER_IE, m_appConfig.TimeToDeleteIETmp * 1000, NULL);
		}
		break;
	case TIMER_COOKIE:
		m_IEControl.ClearTmpFiles();
		if (m_appConfig.DeleteCookies)
		{
			SetTimer(TIMER_COOKIE, m_appConfig.TimeToDeleteCookies * 1000, NULL);
		}
		break;

	case TIMER_CON:
		Redial();
		break; 
	case TIMER_GETIP:
	{
		
	}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CADSLDialUtilityDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{
	return Tray.OnTrayNotification(wParam, lParam);
}


void CADSLDialUtilityDlg::OnShow()
{
	ShowWindow(SW_SHOWDEFAULT);
}


void CADSLDialUtilityDlg::OnExit()
{
	// TODO:  �ڴ���������������
	OnBnClickedButtonHangup();
	CloseWindow();
}


void CADSLDialUtilityDlg::OnAfxIdsScminimize()
{
	ShowWindow(SW_HIDE);
}


void CADSLDialUtilityDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  �ڴ˴������Ϣ����������
	if (nStatus == SW_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
}


void CADSLDialUtilityDlg::OnUpdateAfxIdsScminimize(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	ShowWindow(SW_HIDE);
}


BOOL CADSLDialUtilityDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (wParam == SC_MINIMIZE || lParam == SC_MINIMIZE)
	{
		ShowWindow(FALSE);
	}
	return CDialogEx::OnCommand(wParam, lParam);
}


void CADSLDialUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialogEx::OnSysCommand(nID, lParam);
	if (nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
}


void CADSLDialUtilityDlg::OnDestroy()
{
	OnBnClickedButtonHangup();
	CDialogEx::OnDestroy();
}


afx_msg LRESULT CADSLDialUtilityDlg::OnRedial(WPARAM wParam, LPARAM lParam)
{
	Redial();
	return 0;
}
