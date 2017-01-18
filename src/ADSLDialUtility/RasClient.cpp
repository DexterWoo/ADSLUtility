// RasClient.cpp
// Copyright?1998 Qing Zhang (qzhang7@tfn.net)
// 8/10/98 Create on Win95
// 8/19/98, 11/4/98 Modify on NT4 	
// You are free to use and distribute this source, as long as there is
// no change, and this comment stays intact.

#include "stdafx.h"
#include "RasClient.h"

#pragma message("library is linking with \"rasapi32.lib\"")
#pragma comment(lib, "rasapi32.lib")
CRasClient::CRasClient()
{
	ZeroMemory(&m_rasStatus, sizeof(m_rasStatus));
	m_rasStatus.dwSize = sizeof(m_rasStatus);
	m_ConnectionHandle = NULL;
}

CRasClient::~CRasClient()
{

}

long CRasClient::GetDeviceCount()
{
	DWORD dwSize = 0;
	DWORD dwNumOfDevices = 0;
	RasEnumDevices(NULL, &dwSize, &dwNumOfDevices);
	return dwNumOfDevices;
}

BOOL CRasClient::GetDeviceNameType(int nIndex, 
		CString& strName, CString& strType)
{
	BOOL bResult = FALSE;
	DWORD dwSize = 0;
	DWORD dwNumOfDevices = 0;
	DWORD dwRV = RasEnumDevices(NULL, &dwSize, &dwNumOfDevices);
	if(nIndex >= (int)dwNumOfDevices &&
		nIndex < 0)
	{
		TRACE("BAD DEVICE INDEX!");
		return FALSE;
	}

	RASDEVINFO *lpRdi = new RASDEVINFO[dwNumOfDevices];
	lpRdi->dwSize = sizeof(*lpRdi);
	dwRV = RasEnumDevices(lpRdi, &dwSize, &dwNumOfDevices);
	
	if(dwRV == 0)
	{
		strName = lpRdi[nIndex].szDeviceName;
		strType = lpRdi[nIndex].szDeviceType;
		bResult = TRUE;
	}
	delete []lpRdi;
	return bResult;
}


BOOL CRasClient::GetModemName(CString* strModemNameArray)
{
	DWORD dwSize = 0;
	DWORD dwNumOfDevices = 0;
	DWORD dwRV = RasEnumDevices(NULL, &dwSize, &dwNumOfDevices);

	RASDEVINFO *lpRdi = new RASDEVINFO[dwNumOfDevices];
	lpRdi->dwSize = sizeof(*lpRdi);
	dwRV = RasEnumDevices(lpRdi, &dwSize, &dwNumOfDevices);
	if(dwRV != 0)
	{
		delete []lpRdi;
		return FALSE;
	}

	CString strType;
	int j = 0;
	for(int i = 0; i < (int)dwNumOfDevices; i++)
	{
		strType = lpRdi[i].szDeviceType;
		if(strType.CompareNoCase(_T("MODEM")) == 0)  // = "RASDT_Modem";
		{
			strModemNameArray[j] = lpRdi[i].szDeviceName;
			j++;
		}
	}
	delete []lpRdi;
	return TRUE;
}

long CRasClient::GetModemCount()
{
	DWORD dwSize = 0;
	DWORD dwNumOfDevices = 0;
	DWORD dwRV = RasEnumDevices(NULL, &dwSize, &dwNumOfDevices);

	RASDEVINFO *lpRdi = new RASDEVINFO[dwNumOfDevices];
	lpRdi->dwSize = sizeof(*lpRdi);
	dwRV = RasEnumDevices(lpRdi, &dwSize, &dwNumOfDevices);
	if(dwRV != 0)
	{
		delete []lpRdi;
		return -1;
	}

	CString strType;
	int nModemCount = 0;
	for(int i = 0; i < (int)dwNumOfDevices; i++)
	{
		strType = lpRdi[i].szDeviceType;
		if(strType.CompareNoCase(_T("MODEM")) == 0)  // = "RASDT_Modem";
			nModemCount++;
	}
	delete []lpRdi;
	return nModemCount;
}

DWORD CRasClient::ChangeEntryName(CString strOldName, CString strNewName)
{
	LPTSTR lpszEntry = strOldName.GetBuffer(1);
	strOldName.ReleaseBuffer();
	if(RasValidateEntryName(NULL, lpszEntry) != ERROR_ALREADY_EXISTS)
	{
		// GET ENTRY
		BYTE bDeviceInfo = NULL;
		DWORD dwDeviceInfoSize = sizeof(bDeviceInfo);
		DWORD dwEntrySize = 0;
		DWORD dwRV;
		RASENTRY entry;
		entry.dwSize = sizeof(entry);
		dwRV = RasGetEntryProperties(NULL, lpszEntry, &entry, &dwEntrySize, 
			&bDeviceInfo, &dwDeviceInfoSize);   // GET 603
		dwRV = RasGetEntryProperties(NULL, lpszEntry, &entry, &dwEntrySize, 
			&bDeviceInfo, &dwDeviceInfoSize);   // GET 0
	    if(dwRV == 0)
		{
			LPTSTR lpszNewName = strNewName.GetBuffer(1);
			strNewName.ReleaseBuffer();
			dwRV = RasSetEntryProperties(NULL, (TCHAR*)lpszNewName, &entry, 
				sizeof(entry), NULL, 0);
			return dwRV;
		}
		else
			return dwRV;
	}
	return ERROR_ENTRY_NO_FOUND;
}

DWORD CRasClient::CreateNewEntry(CString strEntry, 
		CString strDeviceType, 
		CString strDeviceName, 
		DWORD dwfNetProtocols, DWORD dwFrameProtocal, 
		CString strLocalPhone,
		CString strPhoneBook /*=NULL*/)
{
	LPTSTR lpszEntry = strEntry.GetBuffer(1);
	strEntry.ReleaseBuffer();
	LPTSTR lpszPhoneBook = strPhoneBook.GetBuffer(1);
	if(strPhoneBook == _T(""))
		lpszPhoneBook = NULL;
	strPhoneBook.ReleaseBuffer();
	if(RasValidateEntryName(lpszPhoneBook, lpszEntry) != ERROR_ALREADY_EXISTS)
	{
		RASENTRY rasEntry;
		::ZeroMemory(&rasEntry, sizeof(rasEntry));
		rasEntry.dwSize = sizeof(rasEntry);
		_tcscpy(rasEntry.szLocalPhoneNumber, strLocalPhone);
		rasEntry.dwfNetProtocols = dwfNetProtocols;   		
		rasEntry.dwFramingProtocol = dwFrameProtocal; 
		// DON'T USE "RASDT_Modem", use "modem", otherwise dwRV = 87
		_tcscpy(rasEntry.szDeviceType, strDeviceType);
		_tcscpy(rasEntry.szDeviceName, strDeviceName);
		DWORD dwRV = RasSetEntryProperties(lpszPhoneBook, (TCHAR*)lpszEntry, &rasEntry, 
			sizeof(rasEntry), NULL, 0);
		return dwRV;
	}
	return ERROR_ALREADY_EXISTS;
}


DWORD CRasClient::SetEntryOption(CString strEntry, DWORD dwfOptions, 
			   BOOL bSet, CString strPhoneBook /*=_T("")*/)
{
	LPTSTR lpszEntry = strEntry.GetBuffer(1);
	strEntry.ReleaseBuffer();
	LPTSTR lpszPhoneBook = strPhoneBook.GetBuffer(1);
	if(strPhoneBook == _T(""))
		lpszPhoneBook = NULL;
	strPhoneBook.ReleaseBuffer();
	RASENTRY entry;
	DWORD dwRV = GetEntryProperties(strEntry, &entry, lpszPhoneBook);
	if(dwRV == 0)
	{
		if(bSet)
			entry.dwfOptions |= dwfOptions; 
		else
			entry.dwfOptions &= ~dwfOptions;
		dwRV = RasSetEntryProperties(lpszPhoneBook, (TCHAR*)lpszEntry, &entry, 
			sizeof(entry), NULL, 0);
	}
	return dwRV;
}

DWORD CRasClient::SetEntryServerProtocal(CString strEntry, 
			   DWORD dwfNetProtocols, CString strPhoneBook /*=_T("")*/)
{
	LPTSTR lpszEntry = strEntry.GetBuffer(1);
	strEntry.ReleaseBuffer();
	LPTSTR lpszPhoneBook = strPhoneBook.GetBuffer(1);
	if(strPhoneBook == _T(""))
		lpszPhoneBook = NULL;
	strPhoneBook.ReleaseBuffer();
	RASENTRY entry;
	DWORD dwRV = GetEntryProperties(strEntry, &entry, lpszPhoneBook);
	if(dwRV == 0)
	{
		entry.dwfNetProtocols = dwfNetProtocols;
		dwRV = RasSetEntryProperties(lpszPhoneBook, (TCHAR*)lpszEntry, &entry,
			sizeof(entry), NULL, 0);
	}
	return dwRV;
}

DWORD CRasClient::SetEntryIPAddress(CString strEntry, int nIPType, 
	BYTE a, BYTE b, BYTE c,	BYTE d, CString strPhoneBook /*= _T("")*/)
{
	LPTSTR lpszEntry = strEntry.GetBuffer(1);
	strEntry.ReleaseBuffer();
	LPTSTR lpszPhoneBook = strPhoneBook.GetBuffer(1);
	if(strPhoneBook == _T(""))
		lpszPhoneBook = NULL;
	strPhoneBook.ReleaseBuffer();
	RASENTRY entry;	
	DWORD dwRV = GetEntryProperties(strEntry, &entry, lpszPhoneBook);
	if(dwRV == 0)
	{
		RASIPADDR ipDns;
		ipDns.a = a;
		ipDns.b = b;
		ipDns.c = c;
		ipDns.d = d;
		switch(nIPType)
		{
			case IPADDRESS :
				entry.dwfOptions |= RASEO_SpecificIpAddr; 
				entry.ipaddr = ipDns;
				break;
			case IPADDRESS_DNS:
				entry.dwfOptions |= RASEO_SpecificNameServers; 
				entry.ipaddrDns = ipDns;
				break;
			case IPADDRESS_DNS_ALT:
				entry.dwfOptions |= RASEO_SpecificNameServers; 
				entry.ipaddrDnsAlt = ipDns;
				break;
			case IPADDRESS_WINS:
				entry.dwfOptions |= RASEO_SpecificNameServers; 
				entry.ipaddrWins = ipDns;
				break;
			case IPADDRESS_WINS_ALT:
				entry.dwfOptions |= RASEO_SpecificNameServers; 
				entry.ipaddrWinsAlt = ipDns;
				break;
		}

		dwRV = RasSetEntryProperties(lpszPhoneBook, (TCHAR*)lpszEntry, &entry,
			sizeof(entry), NULL, 0);
	}
	return dwRV;
}

DWORD CRasClient::GetEntryProperties(CString strEntry, RASENTRY* lpRasEntry
				, LPTSTR lpszPhoneBook)
{
	LPTSTR lpszEntry = strEntry.GetBuffer(1);
	strEntry.ReleaseBuffer();
	if(RasValidateEntryName(lpszPhoneBook, lpszEntry) == ERROR_ALREADY_EXISTS)
	{
		BYTE bDeviceInfo = NULL;
		DWORD dwDeviceInfoSize = sizeof(bDeviceInfo);
		DWORD dwEntrySize = 0;
		DWORD dwRV;
		lpRasEntry->dwSize = sizeof(*lpRasEntry);
		dwRV = RasGetEntryProperties(lpszPhoneBook, lpszEntry, lpRasEntry, &dwEntrySize, 
			&bDeviceInfo, &dwDeviceInfoSize);   // GET 603
		dwRV = RasGetEntryProperties(lpszPhoneBook, lpszEntry, lpRasEntry, &dwEntrySize, 
			&bDeviceInfo, &dwDeviceInfoSize);   // GET 0
	    return dwRV;
	}
	else
		return ERROR_ENTRY_NO_FOUND;
}

DWORD CRasClient::RasDialSyn(CString strEntry, CString strPhoneNumber,
		CString strUserName, CString strPassword, CString strDomain, 
		CString strPhoneBook)
{
	LPTSTR lpszPhoneBook = NULL;
	if(strPhoneBook != _T(""))
	{
		lpszPhoneBook = strPhoneBook.GetBuffer(1);
		strPhoneBook.ReleaseBuffer();
	}
	RASDIALPARAMS dialParams;
    ::ZeroMemory(&dialParams, sizeof(dialParams) );
    dialParams.dwSize = sizeof(dialParams);
	_tcscpy(dialParams.szEntryName, strEntry);
	_tcscpy(dialParams.szPhoneNumber, strPhoneNumber);
	_tcscpy(dialParams.szUserName, strUserName);
	_tcscpy(dialParams.szPassword, strPassword);
	_tcscpy(dialParams.szDomain, strDomain);

	m_ConnectionHandle = NULL;
	DWORD dwError = RasDial(NULL, lpszPhoneBook, &dialParams, NULL, 
		NULL, &m_ConnectionHandle); 
	return dwError;
}


DWORD CRasClient::RasDialAsynCallback1(CString strEntry, LPVOID lpvNotifier, CString strPhoneNumber,
		CString strUserName, CString strPassword, CString strDomain, 
		CString strPhoneBook)
{
	LPTSTR lpszPhoneBook = NULL;
	if(strPhoneBook != _T(""))
	{
		lpszPhoneBook = strPhoneBook.GetBuffer(1);
		strPhoneBook.ReleaseBuffer();
	}
	RASDIALPARAMS dialParams;
    ::ZeroMemory(&dialParams, sizeof(dialParams) );
    dialParams.dwSize = sizeof(dialParams);
	_tcscpy(dialParams.szEntryName, strEntry);
	_tcscpy(dialParams.szPhoneNumber, strPhoneNumber);
	_tcscpy(dialParams.szUserName, strUserName);
	_tcscpy(dialParams.szPassword, strPassword);
	_tcscpy(dialParams.szDomain, strDomain);

	m_ConnectionHandle = NULL;
	DWORD dwError = RasDial(NULL, lpszPhoneBook, &dialParams, 1,  
		lpvNotifier, &m_ConnectionHandle); 
	return dwError;
}


DWORD CRasClient::RasDialAsyn(CString strEntry, LPVOID lpvNotifier, CString strPhoneNumber,
		CString strUserName, CString strPassword, CString strDomain, 
		CString strPhoneBook)
{
	LPTSTR lpszPhoneBook = NULL;
	if(strPhoneBook != _T(""))
	{
		lpszPhoneBook = strPhoneBook.GetBuffer(1);
		strPhoneBook.ReleaseBuffer();
	}
	RASDIALPARAMS dialParams;
    ::ZeroMemory(&dialParams, sizeof(dialParams) );
    dialParams.dwSize = sizeof(dialParams);
	_tcscpy(dialParams.szEntryName, strEntry);
	_tcscpy(dialParams.szPhoneNumber, strPhoneNumber);
	_tcscpy(dialParams.szUserName, strUserName);
	_tcscpy(dialParams.szPassword, strPassword);
	_tcscpy(dialParams.szDomain, strDomain);

	m_ConnectionHandle = NULL;
	DWORD dwError = RasDial(NULL, lpszPhoneBook, &dialParams, 0xFFFFFFFF,  
		lpvNotifier, &m_ConnectionHandle); 
	return dwError;
}


DWORD CRasClient::RasDialAsyn(LPRASDIALPARAMS pParams, LPVOID callback)
{
	m_ConnectionHandle = NULL;
	GetRasConnection(pParams->szEntryName, m_ConnectionHandle);
	if (m_ConnectionHandle != NULL)
	{
		return 0;
	}
	return RasDial(NULL, NULL, pParams, 0, callback, &m_ConnectionHandle);
}

DWORD CRasClient::GetRasErrorString(CString &strError)
{
	strError = _T("Fail to get error string");
	if(m_rasStatus.dwError != 0)
	{
		LPTSTR lpszErr = strError.GetBuffer(256);
		DWORD dwRV = RasGetErrorString(m_rasStatus.dwError, lpszErr, 256);
		strError.ReleaseBuffer();
		return dwRV;
	}
	else 
		return ERROR_NO_RASSTATUS;
}


DWORD CRasClient::GetRasErrorStringByErrorCode(DWORD dwError,
				DWORD dwExtendError, CString &strError)
{
	strError = "Fail to get error string";
	if(dwError != 0)
	{
		LPTSTR lpszErr = strError.GetBuffer(256);
		DWORD dwRV = RasGetErrorString(dwError, lpszErr, 256);
		strError.ReleaseBuffer();
		if(dwExtendError != 0)
		{
			CString strExtend;
			strExtend.Format(_T(" Extended Error Code: %d"), dwExtendError);
			strError = strError + strExtend; 
		}
		return dwRV;
	}
	else 
		return ERROR_NO_RASSTATUS;
}


BOOL CRasClient::GetRasStatusString(CString &strStatus, BOOL& bConnected)
{
	bConnected = FALSE;
	RasGetConnectStatus(m_ConnectionHandle, &m_rasStatus);
	switch(m_rasStatus.rasconnstate)
	{
		case RASCS_Connected:
			strStatus = "Connect to remote server successfully!\n"
				"Please wait...";
			bConnected = TRUE;
			return TRUE;
		case RASCS_Disconnected:
			strStatus = "Disconnected";
			return TRUE;
		case RASCS_OpenPort:
			strStatus = "Opening the port...";
			break;
		case RASCS_PortOpened:
			strStatus = "Port has been opened successfully";
			break;
		case RASCS_ConnectDevice:
			strStatus = "Connecting to the device...";
			break;
		case RASCS_DeviceConnected:
			strStatus = "The device has connected successfully.";
			break;
		case RASCS_AllDevicesConnected:
			strStatus = "All devices in the device chain have successfully connected.";
			break;
		case RASCS_Authenticate:
			strStatus = "Verifying the user name and password...";  
			break;
		case RASCS_AuthNotify:
			strStatus = "An authentication event has occurred.";
			break;
		case RASCS_AuthRetry:
			strStatus = "Requested another validation attempt with a new user.";
			break;
		case RASCS_AuthCallback:
			strStatus = "Server has requested a callback number.";
			break;
		case RASCS_AuthChangePassword:
			strStatus = "The client has requested to change the password";
			break;
		case RASCS_AuthProject:
			// strStatus = "The projection phase is starting...";
			strStatus = "Registering your computer on the network...";
			break;
		case RASCS_AuthLinkSpeed:
			strStatus = "The link-speed calculation phase is starting...";
			break;
		case RASCS_AuthAck:
			strStatus = "An authentication request is being acknowledged."; 
			break;
		case RASCS_ReAuthenticate:
			strStatus = "Reauthentication (after callback) is starting. ";
			break;
		case RASCS_Authenticated:
			strStatus = "The client has successfully completed authentication.";
			break;
		case RASCS_PrepareForCallback:
			strStatus = "The line is about to disconnect for callback.";
			break;
		case RASCS_WaitForModemReset:
			strStatus = "Delaying to give the modem time to reset for callback.";
			break;
		case RASCS_WaitForCallback:
			strStatus = "Waiting for an incoming call from server.";
			break;
		case RASCS_Projected:
			strStatus = "Projection result information is available.";
			break;
		case RASCS_StartAuthentication:
			strStatus = "User authentication is being initiated or retried.";  
			break;
		case RASCS_CallbackComplete:
			strStatus = "Client has been called back and is about to resume authentication.";
			break;
		case RASCS_LogonNetwork:
			strStatus = "Logging on to the network...";
			break;
		case RASCS_SubEntryConnected:
			strStatus = "Subentry has been connected";
			break;
		case RASCS_SubEntryDisconnected:
			strStatus = "Subentry has been disconnected";
			break;
		case RASCS_Interactive:
			strStatus = "Terminal state supported by RASPHONE.EXE.";
			break;
		case RASCS_RetryAuthentication:
			strStatus = "Retry authentication state supported by RASPHONE.EXE.";
			break;
		case RASCS_CallbackSetByCaller:
			strStatus = "Callback state supported by RASPHONE.EXE.";
			break;
		case RASCS_PasswordExpired:
			strStatus = "Change password state supported by RASPHONE.EXE.";
			break;
	}
	//if(m_rasStatus.dwError != 0)		// 9/14/98 FOR WIN95/98
	//	return TRUE;
	//else
	return FALSE;
}

BOOL CRasClient::HangUpConnection(CString strEntryName)
{
	if(m_ConnectionHandle == NULL)
	{
		HRASCONN hrasConn;
		if(GetRasConnection(strEntryName, hrasConn))
		{
			if(RasHangUp(hrasConn) == 0)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
	{
		if (RasHangUp(m_ConnectionHandle) == ERROR_SUCCESS)
		{
			RASCONNSTATUS status = { 0 };
			status.dwSize = sizeof(RASCONNSTATUS);
			while (RasGetConnectStatus(m_ConnectionHandle, &status) == ERROR_SUCCESS)
			{
				if (status.rasconnstate == RASCS_Disconnected)
				{
					break;
				}
				Sleep(0);
				RasHangUp(m_ConnectionHandle);
			}
			m_ConnectionHandle = NULL;

			return TRUE;
		}
		else
			return FALSE;
	}
}

BOOL CRasClient::GetRasConnection(CString strEntryName, HRASCONN & hrasConn)
{
	BOOL bFind = FALSE;
	CString strEntry;
	DWORD dwSizeBuf = 0;
	DWORD dwConnections = 0;
	RASCONN rasConn = { 0 };
	rasConn.dwSize = sizeof(rasConn);
	DWORD dw = RasEnumConnections(&rasConn, &dwSizeBuf, &dwConnections);
	if(dwConnections <= 0)
		return FALSE;
	RASCONN *lpRasConn = new RASCONN[dwConnections];
	lpRasConn->dwSize = sizeof(*lpRasConn);
	RasEnumConnections(lpRasConn, &dwSizeBuf, &dwConnections);
	for(DWORD i = 0; i < dwConnections; i++)
	{
		strEntry = lpRasConn[i].szEntryName;
		if(strEntry.CompareNoCase(strEntryName) == 0)
		{
			hrasConn = lpRasConn[i].hrasconn;
			bFind = TRUE;
		}
	}
	delete []lpRasConn;
	if(bFind)
		return TRUE;
	else
		return FALSE;
}

CString CRasClient::GetCurIP()
{
	CString strIP;
	if (m_ConnectionHandle != NULL)
	{
		RASPPPIP rip = { 0 };
		DWORD ll;
		rip.dwSize = sizeof(RASPPPIP);
		DWORD dwRet = RasGetProjectionInfo(m_ConnectionHandle, RASP_PppIp,
			(LPVOID)&rip, (LPDWORD)&ll);
		if (dwRet == ERROR_SUCCESS)
		{
			strIP = rip.szIpAddress;
		}
		else
		{
			GetLastError(); 
		}
	}
	return strIP;
}
