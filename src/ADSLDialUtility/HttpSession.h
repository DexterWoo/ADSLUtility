#pragma once
#include<afxinet.h>

class CHttpSession :
	public CInternetSession
{
public:
	CHttpSession(DWORD dwTimeout = 60000, LPCTSTR szAgentName = NULL,  DWORD dwContext = 1, DWORD dwAccessType =INTERNET_OPEN_TYPE_PRECONFIG);
	~CHttpSession();

protected:
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	// Operations
	void SetStatus(LPCTSTR fmt, ...);
};

