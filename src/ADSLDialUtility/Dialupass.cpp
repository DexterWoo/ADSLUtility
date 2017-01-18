// Dialupass.cpp: implementation of the CDialupass class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Dialupass.h"
#include <Wtsapi32.h>
#include "Log.h"
#pragma comment(lib, "Wtsapi32.lib")
//#include "until.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCHAR *GetCurrentLoginUser()
{
	TCHAR	*szLogName = NULL;
	DWORD	dwSize = 0;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSUserName, &szLogName, &dwSize))
	{
		TCHAR	*lpUser = new TCHAR[256];
		lstrcpy(lpUser, szLogName);
		WTSFreeMemory(szLogName);
		return lpUser;
	}
	else
		return NULL;

}

CDialupass::CDialupass()
{
	m_nMax = 0;
	m_lpCurrentUser = GetCurrentLoginUser();
	m_nRasCount = GetRasEntryCount();
	m_PassWords = new PASSWORDS[m_nRasCount];
	OneInfo = new COneInfo*[m_nRasCount];
	m_nUsed = 0;
	m_nCount = 0;
	GetRasEntries();
}

CDialupass::~CDialupass()
{
	for (int i = 0; i < m_nRasCount; i++)
		delete OneInfo[i];
	delete[] OneInfo;
	delete m_PassWords;
	if (m_lpCurrentUser != NULL)
		delete[] m_lpCurrentUser;
}

COneInfo* CDialupass::GetOneInfo(const GUID& guid)
{
	BYTE* pbBuffer = (BYTE*)&guid;
	CString strGuid;
	CString strTmp;
	for (int i = 0; i < 16; i++)
	{
		strTmp.Format(_T("%02X"), pbBuffer[i]);
		strGuid += strTmp;
	}
	Log.error("GetOneInfo", "Guid look: %S", strGuid);
	for (int i = 0; i < m_nMax; i++)
	{
		Log.error("GetOneInfo", "Guid: %S", OneInfo[i]->Get(STR_GUID));
		if (strGuid.CompareNoCase(OneInfo[i]->Get(STR_GUID)) == 0)
		{
			return OneInfo[i];
		}
	}
	return NULL;
}

DWORD CDialupass::GetRasEntryCount()
{
	int		nCount = 0;
	TCHAR	*lpPhoneBook[2];
	TCHAR	szPhoneBook1[MAX_PATH + 1], szPhoneBook2[MAX_PATH + 1];
	GetWindowsDirectory(szPhoneBook1, sizeof(szPhoneBook1));
	lstrcpy(_tcsrchr(szPhoneBook1, _T('\\')) + 1, _T("Documents and Settings\\"));
	lstrcat(szPhoneBook1, m_lpCurrentUser);
	lstrcat(szPhoneBook1, _T("\\Application Data\\Microsoft\\Network\\Connections\\pbk\\rasphone.pbk"));
	SHGetSpecialFolderPath(NULL, szPhoneBook2, 0x23, 0);
	wsprintf(szPhoneBook2, _T("%s\\%s"), szPhoneBook2, _T("Microsoft\\Network\\Connections\\pbk\\rasphone.pbk"));

	lpPhoneBook[0] = szPhoneBook1;
	lpPhoneBook[1] = szPhoneBook2;

	DWORD	nSize = 1024 * 4;
	TCHAR	*lpszReturnBuffer = new TCHAR[nSize];

	for (int i = 0; i < sizeof(lpPhoneBook) / sizeof(int); i++)
	{
		memset(lpszReturnBuffer, 0, nSize);
		
		GetPrivateProfileSectionNames(lpszReturnBuffer, nSize, lpPhoneBook[i]);
		for (TCHAR *lpSection = lpszReturnBuffer; *lpSection != '\0'; lpSection += lstrlen(lpSection) + 1)
		{
			nCount++;
		}
	}
	delete lpszReturnBuffer;
	return nCount;
}

LPTSTR CDialupass::GetLocalSid()
{
	union
	{
		SID s;
		TCHAR c[256];
	}Sid;
	DWORD sizeSid = sizeof(Sid);
	TCHAR DomainName[256];
	DWORD sizeDomainName = sizeof(DomainName);
	SID_NAME_USE peUse;
	LPTSTR pSid = NULL;


	if (m_lpCurrentUser == NULL)
		return NULL;

	if (!LookupAccountName(NULL, m_lpCurrentUser, (SID*)&Sid, &sizeSid, DomainName, &sizeDomainName, &peUse))return NULL;
	if (!IsValidSid(&Sid))return NULL;


	typedef BOOL(WINAPI *ConvertSid2StringSid)(PSID, LPTSTR *);
	ConvertSid2StringSid proc;
	HINSTANCE	hLibrary = LoadLibrary(_T("advapi32.dll"));
	proc = (ConvertSid2StringSid)GetProcAddress(hLibrary, "ConvertSidToStringSidW");
	if (proc)   proc((SID*)&Sid.s, &pSid);
	FreeLibrary(hLibrary);
	return pSid;
}


void CDialupass::StringToLsaStr(LPTSTR strValue, PLSA_UNICODE_STRING lsa)
{
	lsa->Length = lstrlen(strValue)*2;
	lsa->MaximumLength = lsa->Length + 2;
	lsa->Buffer = (PWSTR)malloc(lsa->MaximumLength);
	wcscpy(lsa->Buffer, strValue);
	//MultiByteToWideChar(NULL, NULL, (LPCSTR)AValue, lstrlen(AValue), lsa->Buffer, lsa->MaximumLength);
}


PLSA_UNICODE_STRING CDialupass::GetLsaData(LPTSTR KeyName)
{
	LSA_OBJECT_ATTRIBUTES LsaObjectAttribs;
	LSA_HANDLE LsaHandle;
	LSA_UNICODE_STRING LsaKeyName;
	NTSTATUS nts;
	PLSA_UNICODE_STRING OutData;

	ZeroMemory(&LsaObjectAttribs, sizeof(LsaObjectAttribs));
	nts = LsaOpenPolicy(NULL, &LsaObjectAttribs, POLICY_GET_PRIVATE_INFORMATION, &LsaHandle);
	if (nts != 0)return NULL;
	StringToLsaStr(KeyName, &LsaKeyName);
	nts = LsaRetrievePrivateData(LsaHandle, &LsaKeyName, &OutData);
	if (nts != 0)return NULL;
	nts = LsaClose(LsaHandle);
	if (nts != 0)return NULL;
	return OutData;
}
/////////
void CDialupass::ParseLsaBuffer(LPCWSTR Buffer, USHORT Length)
{
	TCHAR chr, PswStr[256];
	PswStr[0] = 0;

	for (int i = 0, SpacePos = 0, TXT = 0; i < Length / 2 - 2; i++)
	{
		chr = Buffer[i];
		if (chr == 0)
		{
			SpacePos++;
			switch (SpacePos)
			{
			case 1:
				PswStr[TXT] = chr;
				_tcscpy_s(m_PassWords[m_nUsed].UID, PswStr);
				break;
			case 6:
				PswStr[TXT] = chr;
				_tcscpy_s(m_PassWords[m_nUsed].login, PswStr);
				break;
			case 7:
				PswStr[TXT] = chr;
				_tcscpy_s(m_PassWords[m_nUsed].pass, PswStr);
				m_PassWords[m_nUsed].used = false;
				m_nUsed++;
				break;
			}
			ZeroMemory(PswStr, 256);
			TXT = 0;
		}
		else
		{
			PswStr[TXT] = chr;
			TXT++;
		}
		if (SpacePos == 9)SpacePos = 0;
	}
}
///////////
void CDialupass::GetLsaPasswords()
{
	PLSA_UNICODE_STRING PrivateData;
	TCHAR Win2k[] = _T("RasDialParams!%s#0");
	TCHAR WinXP[] = _T("L$_RasDefaultCredentials#0");
	TCHAR temp[256];

	wsprintf(temp, Win2k, GetLocalSid());

	PrivateData = GetLsaData(temp);
	if (PrivateData != NULL)
	{
		ParseLsaBuffer(PrivateData->Buffer, PrivateData->Length);
		LsaFreeMemory(PrivateData);
		PrivateData = NULL;
	}
	PrivateData = GetLsaData(WinXP);
	if (PrivateData != NULL)
	{
		ParseLsaBuffer(PrivateData->Buffer, PrivateData->Length);
		LsaFreeMemory(PrivateData);
		PrivateData = NULL;
	}
}


bool CDialupass::GetRasEntries()
{

	int		nCount = 0;
	TCHAR	*lpPhoneBook[2];
	TCHAR	szPhoneBook1[MAX_PATH + 1], szPhoneBook2[MAX_PATH + 1];
	GetWindowsDirectory(szPhoneBook1, sizeof(szPhoneBook1));
	lstrcpy(_tcsrchr(szPhoneBook1, _T('\\')) + 1, _T("Documents and Settings\\"));
	lstrcat(szPhoneBook1, m_lpCurrentUser);
	lstrcat(szPhoneBook1, _T("\\Application Data\\Microsoft\\Network\\Connections\\pbk\\rasphone.pbk"));
	SHGetSpecialFolderPath(NULL, szPhoneBook2, 0x23, 0);
	wsprintf(szPhoneBook2, _T("%s\\%s"), szPhoneBook2, _T("Microsoft\\Network\\Connections\\pbk\\rasphone.pbk"));

	lpPhoneBook[0] = szPhoneBook1;
	lpPhoneBook[1] = szPhoneBook2;


	OSVERSIONINFO osi;
	osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osi);

	if (osi.dwPlatformId == VER_PLATFORM_WIN32_NT && osi.dwMajorVersion >= 5)
	{
		GetLsaPasswords();
	}

	DWORD	nSize = 1024 * 4;
	TCHAR	*lpszReturnBuffer = new TCHAR[nSize];

	for (int i = 0; i < sizeof(lpPhoneBook) / sizeof(int); i++)
	{
// 		CINIData ini(lpPhoneBook[i]);
// 		ini.SetCodepage(CP_UTF8);
// 		CStringArray strArray;
// 		ini.GetSections(strArray);

		memset(lpszReturnBuffer, 0, nSize);

		GetPrivateProfileSectionNames(lpszReturnBuffer, nSize, lpPhoneBook[i]);
		for (TCHAR *lpSection = lpszReturnBuffer; *lpSection != '\0'; lpSection += lstrlen(lpSection) + 1)
		{
			TCHAR	strDialParamsUID[256];
			TCHAR	strUserName[256];
			TCHAR	strPassWord[256];
			TCHAR	strPhoneNumber[256];
			TCHAR	strDevice[256];
			TCHAR   strGUID[256];
			memset(strDialParamsUID, 0, sizeof(strDialParamsUID));
			memset(strUserName, 0, sizeof(strUserName));
			memset(strPassWord, 0, sizeof(strPassWord));
			memset(strPhoneNumber, 0, sizeof(strPhoneNumber));
			memset(strDevice, 0, sizeof(strDevice));


			int	nBufferLen = GetPrivateProfileString(lpSection, _T("DialParamsUID"), 0,
				strDialParamsUID, sizeof(strDialParamsUID), lpPhoneBook[i]);

			if (nBufferLen > 0)//DialParamsUID=4326020    198064
			{
				for (int j = 0; j < (int)m_nRasCount; j++)
				{
					if (lstrcmp(strDialParamsUID, m_PassWords[j].UID) == 0)
					{
						lstrcpy(strUserName, m_PassWords[j].login);
						lstrcpy(strPassWord, m_PassWords[j].pass);
						m_PassWords[j].used = true;
						m_nUsed++;
						break;
					}
				}
			}

			GetPrivateProfileString(lpSection, _T("PhoneNumber"), 0,
				strPhoneNumber, sizeof(strDialParamsUID), lpPhoneBook[i]);
			GetPrivateProfileString(lpSection, _T("Device"), 0,
				strDevice, sizeof(strDialParamsUID), lpPhoneBook[i]);
			GetPrivateProfileString(lpSection, _T("GUID"), 0,
				strGUID, sizeof(strGUID), lpPhoneBook[i]);

			TCHAR	*lpRealSection = (TCHAR *)UTF8ToGB2312(lpSection);
			TCHAR *lpRealDevice = (TCHAR *)UTF8ToGB2312(strDevice);
			TCHAR *lpRealUserName = (TCHAR *)UTF8ToGB2312(strUserName);
			TCHAR *lpRealGUID = (TCHAR *)UTF8ToGB2312(strGUID);
			Set(strDialParamsUID, lpRealSection, lpRealDevice, strPassWord,
				strPhoneNumber, lpRealUserName, lpRealGUID);
			if (_tcslen(lpRealSection) > 0)
			{
				delete[] lpRealSection;
			}
			if (_tcslen(lpRealDevice) > 0)
			{
				delete[] lpRealDevice;
			}
			if (_tcslen(lpRealUserName) > 0)
			{
				delete[] lpRealUserName;
			}
			if (_tcslen(lpRealGUID) > 0)
			{
				delete[] lpRealGUID;
			}
		}
	}
	delete lpszReturnBuffer;

	return true;
}

BOOL CDialupass::Set(TCHAR *DialParamsUID, TCHAR *Name, TCHAR *User, TCHAR *Password, TCHAR *PhoneNumber, TCHAR *Device, TCHAR* strGuid)
{
	for (int i = 0; i < m_nMax; i++){
		if (0 == _tcscmp(OneInfo[i]->Get(STR_DialParamsUID), DialParamsUID)){

			if (Name != NULL)
				OneInfo[i]->Set(STR_Name, Name);
			if (User != NULL)
				OneInfo[i]->Set(STR_User, User);
			if (Password != NULL)
				OneInfo[i]->Set(STR_Password, Password);
			if (PhoneNumber != NULL)
				OneInfo[i]->Set(STR_PhoneNumber, PhoneNumber);
			if (Device != NULL)
				OneInfo[i]->Set(STR_Device, Device);
			if (strGuid != NULL)
				OneInfo[i]->Set(STR_GUID, strGuid);
			return TRUE;
		}
	}

	if (m_nMax < m_nRasCount){

		OneInfo[m_nMax] = new COneInfo;
		OneInfo[m_nMax]->Set(STR_DialParamsUID, DialParamsUID);
		OneInfo[m_nMax]->Set(STR_Name, Name);
		OneInfo[m_nMax]->Set(STR_User, User);
		OneInfo[m_nMax]->Set(STR_Password, Password);
		OneInfo[m_nMax]->Set(STR_PhoneNumber, PhoneNumber);
		OneInfo[m_nMax]->Set(STR_Device, Device);
		OneInfo[m_nMax]->Set(STR_GUID, strGuid);
		m_nMax++;
		return TRUE;
	}
	return false;
}

LPCTSTR CDialupass::UTF8ToGB2312(WCHAR UTF8Str[])
{
	CW2A pszA(UTF8Str);

	//wcstombs(strKey, UTF8Str, _tcslen(UTF8Str) * 2);
	if (pszA == NULL || lstrlenA(pszA) == 0)
		return _T("");
	int	nStrLen = lstrlenA(pszA) * 2+2;
	TCHAR *lpWideCharStr = new TCHAR[nStrLen];

	MultiByteToWideChar(CP_UTF8, 0, pszA, -1, (LPWSTR)lpWideCharStr, nStrLen);

	return  lpWideCharStr;
}