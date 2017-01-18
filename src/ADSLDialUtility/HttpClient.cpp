#include "stdafx.h"
#include "HttpClient.h"
#include "HttpSession.h"
#include "Log.h"
#include "atlconv.h"

CHttpClient::CHttpClient()
{
}


CHttpClient::~CHttpClient()
{
}

std::string CHttpClient::HttpGet(const CString& strUrl, const CString& strFilePath /*= ""*/)
{
	std::string strRet;
	CHttpSession* pHttpSession = NULL;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;

	TRY
	{
		pHttpSession = new CHttpSession();

		CString strServer, strObject;
		DWORD dwServiceType;
		unsigned short nPort;
		AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
		pHttpConnection = pHttpSession->GetHttpConnection(strServer, nPort);
		DWORD HttpRequestFlags = 0;
		if (dwServiceType == AFX_INET_SERVICE_HTTPS)
			HttpRequestFlags = INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE;
		else
			HttpRequestFlags = INTERNET_FLAG_NO_COOKIES;

	_RESEND____:
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, HTTP_VERSION, HttpRequestFlags);
		pHttpFile->SendRequest();

		DWORD dwRetCode;
		pHttpFile->QueryInfoStatusCode(dwRetCode);
		if (dwRetCode != 200)
		{
			DWORD dwErrorCode = GetLastError();
			DWORD dwError = pHttpFile->ErrorDlg(NULL, dwErrorCode,
				FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA,
				NULL);

			if (dwError == ERROR_INTERNET_FORCE_RETRY)
				goto _RESEND____;

			throw new CInternetException(dwError);
		}

		if (strFilePath.IsEmpty())
		{
			ReadDataFromServer(pHttpFile, strRet);
		}
		else
		{
			StreamToFile(pHttpFile, strFilePath);
		}
	}
	CATCH(CException, e)
	{
		TCHAR szError[256] = { 0 };
		e->GetErrorMessage(szError, 255);
		Log.error(_T("HttpGet"), _T("Error:%s||| %s"), szError, strUrl);
		strRet = CW2A(szError);
	}
	END_CATCH
	
	if (pHttpFile != NULL)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConnection != NULL)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
	}
	if (pHttpSession != NULL)
	{
		pHttpSession->Close();
		delete pHttpSession;
	}
	return strRet;
}


void CHttpClient::StreamToFile(CHttpFile* pHttpFile, const CString& strFilePath)
{
	TCHAR buff[64] = { 0 };
	DWORD dwSizeOfRq = sizeof(buff);
	long long llContLen = 0;
	if (pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
	{
		llContLen = _ttol(buff);
	}
	CFile file;
	file.Open(strFilePath, CFile::modeWrite | CFile::shareDenyWrite | CFile::modeNoTruncate | CFile::modeCreate);
	const UINT httpsBufferSize = 64 * 1024;
	UINT lread = httpsBufferSize;
	CSmartArrayPtr<BYTE> pBuffer;
	pBuffer = new BYTE[lread + 1];
	while (lread > 0)
	{
		lread = pHttpFile->Read((BYTE*)pBuffer, httpsBufferSize);
		file.Write((BYTE*)pBuffer, lread);
	}
	file.Close();
}

void CHttpClient::ReadDataFromServer(CHttpFile* pHttpFile, std::string &strRet)
{
	const UINT httpsBufferSize = 64 * 1024;
	UINT lread = httpsBufferSize;
	CSmartArrayPtr<BYTE> pBuffer;
	pBuffer = new BYTE[lread + 1];
	while (lread > 0)
	{
		lread = pHttpFile->Read((BYTE*)pBuffer, httpsBufferSize);
		pBuffer[lread] = '\0';
		strRet.append((char*)(BYTE*)pBuffer);
	}
}


/*
std::string CHttpClient::HttpPost(const CString& strUrl, CSmartArrayPtr<BYTE> postData, const DWORD dwContentLength)
{
	std::string strRet;
	BOOL bRet = TRUE;
	CHttpSession* pHttpSession = NULL;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	TRY
	{
		pHttpSession = new CHttpSession();

		CString strServer, strObject;
		DWORD dwServiceType;
		unsigned short nPort;
		AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
		pHttpConnection = pHttpSession->GetHttpConnection(strServer, nPort);
		DWORD HttpRequestFlags = 0;
		if (dwServiceType == AFX_INET_SERVICE_HTTPS)
			HttpRequestFlags = INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE;
		else
			HttpRequestFlags = INTERNET_FLAG_NO_COOKIES;

		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,
			strObject, NULL, 1, NULL, HTTP_VERSION, HttpRequestFlags);

		CString strHeaders = _T("Content-Type:application/xml\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n");
		strHeaders += _T("Connection:Keep-Alive\r\nKeep-Alive: timeout=60\r\n");

		pHttpFile->AddRequestHeaders(strHeaders, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	_RESEND____:
		pHttpFile->SendRequestEx(dwContentLength, HSR_INITIATE, 1);


		pHttpFile->Write((BYTE*)postData, dwContentLength);
		pHttpFile->EndRequest();
		DWORD dwRet;
		pHttpFile->QueryInfoStatusCode(dwRet);
		if (dwRet != 200)
		{
			DWORD dwErrorCode = GetLastError();
			DWORD dwError = pHttpFile->ErrorDlg((AfxGetApp()->GetMainWnd()), dwErrorCode,
				FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA,
				NULL);

			if (dwError == ERROR_INTERNET_FORCE_RETRY)
				goto _RESEND____;
			throw new CInternetException(dwError);
		}

		ReadDataFromServer(pHttpFile, strRet);

	}
		CATCH(CInternetException, e)
	{

		TCHAR szError[256] = { 0 };
		e->GetErrorMessage(szError, 255);
	}
	END_CATCH

		return strRet;
}
*/
