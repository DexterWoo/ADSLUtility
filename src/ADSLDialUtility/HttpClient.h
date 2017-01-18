#pragma once
#include <string>
#include "SmartPtr.h"
#include "HttpSession.h"
class CHttpClient
{
public:
	CHttpClient();
	~CHttpClient();
	std::string HttpGet(const CString& strUrl, const CString& strFilePath = _T(""));

protected:
	void ReadDataFromServer(CHttpFile* pHttpFile, std::string &strRet);
	void StreamToFile(CHttpFile* pHttpFile, const CString& strFilePath);
};

