#include "stdafx.h"
#include "RasControl.h"
#include "Log.h"
CWnd* CRasControl::pWnd = NULL;
CRasControl::CRasControl()
{
	InitEntrylist();
}


CRasControl::~CRasControl()
{
	for each (LPRASDIALPARAMS p in m_entryList)
	{
		delete p;
	}
	m_entryList.clear();
	delete m_dialPass;
	delete m_rasClient;
}

UINT CRasControl::GetEntryCount()
{
	return m_entryList.size();
}

void CRasControl::Dial(int index)
{
	RASDIALPARAMS* pDialParam = m_entryList[index];
	m_rasClient->RasDialAsyn(pDialParam, RasDialFunc);
}

CString CRasControl::GetEntryName(int index)
{
	return m_entryList.at(index)->szEntryName;
}

CString CRasControl::GetEntryUserName(int index)
{
	return m_entryList.at(index)->szUserName;
}

CString CRasControl::GetEntryPassword(int index)
{
	return m_entryList.at(index)->szPassword;
}

CString CRasControl::GetCurIP()
{
	return m_rasClient->GetCurIP();
}

void CRasControl::HangUp(int index)
{
	m_rasClient->HangUpConnection(m_entryList.at(index)->szEntryName);
}
static DWORD GetRasEntries(LPRASENTRYNAME& rasEntryNames, DWORD& bufferSize, DWORD& entries)
{
	if (rasEntryNames)
	{
		// Deallocate memory for the entries buffer
		HeapFree(GetProcessHeap(), 0, rasEntryNames);
		rasEntryNames = 0;
	}

	rasEntryNames = (LPRASENTRYNAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);

	if (!rasEntryNames)
	{
		Log.error("GetRasEntries", "HeapAlloc failed when trying to enumerate RAS connections");
		throw 0;
	}

	// The first RASENTRYNAME structure in the array must contain the structure size
	rasEntryNames[0].dwSize = sizeof(RASENTRYNAME);

	// Call RasEnumEntries to enumerate all RAS entry names
	return RasEnumEntries(0, 0, rasEntryNames, &bufferSize, &entries);
}

void CRasControl::InitEntrylist()
{
	for each (LPRASDIALPARAMS p in m_entryList)
	{
		delete p;
	}
	m_entryList.clear();
	m_dialPass = new CDialupass;
	m_rasClient = new CRasClient;
	DWORD dwRet = ERROR_SUCCESS;
	DWORD dwEntries = 0;
	DWORD dwCb = sizeof(RASENTRYNAME);
	LPRASENTRYNAME lpRasEntryName = NULL;

	
	dwRet = GetRasEntries(lpRasEntryName, dwCb, dwEntries);
	

	if (dwRet == ERROR_BUFFER_TOO_SMALL){
		dwRet = GetRasEntries(lpRasEntryName, dwCb, dwEntries);
		if (ERROR_SUCCESS != dwRet){
			Log.error("InitEntrylist", "error code: %d, lasterr: %d", dwRet, GetLastError());
			return;
		}
	}
	if (dwRet == ERROR_SUCCESS)
	{
		// The first RASENTRYNAME structure in the array must contain the structure size
		lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

		// Call RasEnumEntries to enumerate all RAS entry names
		dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);
		
		// If successful, print the RAS entry names 
		if (ERROR_SUCCESS == dwRet){
			TRACE(L"The following RAS entry names were found:\n");
			for (DWORD i = 0; i < dwEntries; i++){
				TRACE(L"%s\n", lpRasEntryName[i].szEntryName);
				LPRASDIALPARAMS params = new RASDIALPARAMS;
				ZeroMemory(params, sizeof(RASDIALPARAMS));
				wcscpy(params->szEntryName, lpRasEntryName[i].szEntryName);
				params->dwSize = sizeof(RASDIALPARAMS);
				BOOL bPwd = FALSE;
				DWORD dwRet = RasGetEntryDialParams(NULL, params, &bPwd);
				RASENTRY rasEntry = { 0 };
				rasEntry.dwSize = sizeof(RASENTRY);
				DWORD dwIOSize = rasEntry.dwSize;
				dwRet = RasGetEntryProperties(lpRasEntryName[i].szPhonebookPath, lpRasEntryName[i].szEntryName, &rasEntry, &dwIOSize, NULL, NULL);
				COneInfo* one = m_dialPass->GetOneInfo(rasEntry.guidId);	
				if (one != NULL)
				{
					_tcscpy(params->szPassword, one->Get(STR_Password));
					m_entryList.push_back(params);
				}
			}
		}

		//Deallocate memory for the connection buffer
		HeapFree(GetProcessHeap(), 0, lpRasEntryName);
		lpRasEntryName = NULL;
	}
	else
	{
		Log.error("InitEntrylist", "error code: %d, lasterr: %d", dwRet, dwCb);
	}
}

void CALLBACK RasDialFunc(_In_ UINT unMsg, _In_ RASCONNSTATE rasconnstate, _In_ DWORD dwError)
{
	if (CRasControl::pWnd != NULL)
		PostMessage(CRasControl::pWnd->m_hWnd, WM_DIALSTATE_CHANGED, rasconnstate, dwError);
}
