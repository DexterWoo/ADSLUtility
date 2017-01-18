#include "stdafx.h"
#include "HttpSession.h"


CHttpSession::CHttpSession(DWORD dwTimeout /*= 60000*/, LPCTSTR szAgentName /*= NULL*/, DWORD dwContext /*= 1*/, DWORD dwAccessType /*=INTERNET_OPEN_TYPE_PRECONFIG*/) :CInternetSession(szAgentName, dwContext, dwAccessType, 
					NULL,//pstrProxyName, 
					NULL,//pstrProxyBypass, 
					0)//dwFlags) 
{
	TRY {
		BOOL bRet = FALSE;
		bRet = EnableStatusCallback(TRUE);
		bRet = SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwTimeout);
		bRet = SetOption(INTERNET_OPTION_SEND_TIMEOUT, dwTimeout);
		bRet = SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwTimeout);
		bRet = SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, dwTimeout);
		bRet = SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwTimeout);
		bRet = SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);
	}
	CATCH (CInternetException, internetExcep)
	{
		
	}
	END_CATCH
}


CHttpSession::~CHttpSession()
{
}


void CHttpSession::OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	// TODO: Add your specialized code here and/or call the base class

	// Status callbacks need thread-state protection. 
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CString str;

	switch (dwInternetStatus)
	{
	case INTERNET_STATUS_RESOLVING_NAME:
		str.Format(_T("Resolving name for %s"), lpvStatusInformation);
		break;

	case INTERNET_STATUS_NAME_RESOLVED:
		str.Format(_T("Resolved name for %s"), lpvStatusInformation);
		break;

	case INTERNET_STATUS_HANDLE_CREATED:
		//str.Format("Handle %8.8X created", hInternet);
		break;

	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		str.Format(_T("Connecting to socket address "));
		break;

	case INTERNET_STATUS_REQUEST_SENT:
		str.Format(_T("Request sent"));
		break;

	case INTERNET_STATUS_SENDING_REQUEST:
		str.Format(_T("Sending request..."));
		break;

	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		str.Format(_T("Connected to socket address"));
		break;

	case INTERNET_STATUS_RECEIVING_RESPONSE:
		return;
		str.Format(_T("Receiving response..."));
		break;

	case INTERNET_STATUS_RESPONSE_RECEIVED:
		str.Format(_T("Response received"));
		break;

	case INTERNET_STATUS_CLOSING_CONNECTION:
		str.Format(_T("Closing the connection to the server"));
		break;

	case INTERNET_STATUS_CONNECTION_CLOSED:
		str.Format(_T("Connection to the server closed"));
		break;

	case INTERNET_STATUS_HANDLE_CLOSING:
		return;
		str.Format(_T("Handle closed"));
		break;

	case INTERNET_STATUS_REQUEST_COMPLETE:
		// See the CInternetSession constructor for details on INTERNET_FLAG_ASYNC.
		// The lpvStatusInformation parameter points at an INTERNET_ASYNC_RESULT 
		// structure, and dwStatusInformationLength contains the final completion 
		// status of the asynchronous function. If this is ERROR_INTERNET_EXTENDED_ERROR, 
		// the application can retrieve the server error information by using the 
		// Win32 function InternetGetLastResponseInfo. See the ActiveX SDK for more 
		// information about this function. 
		if (dwStatusInformationLength == sizeof(INTERNET_ASYNC_RESULT))
		{
			INTERNET_ASYNC_RESULT* pResult = (INTERNET_ASYNC_RESULT*)lpvStatusInformation;
			str.Format(_T("Request complete, dwResult = %8.8X, dwError = %8.8X"),
				pResult->dwResult, pResult->dwError);
		}
		else
			str.Format(_T("Request complete"));
		break;

	case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
	case INTERNET_STATUS_REDIRECT:
	default:
		str.Format(_T("Unknown status: %d"), dwInternetStatus);
		break;
	}

	SetStatus(str);
}

void CHttpSession::SetStatus(LPCTSTR fmt, ...)
{
	va_list args;
	TCHAR buffer[512];

	va_start(args, fmt);
	_vstprintf_s(buffer, fmt, args);
	va_end(args);
#ifdef UNICODE
	TRACE("CHttpSession::SetStatus: %S\n", buffer);
#else
	TRACE("CHttpSession::SetStatus: %s\n", buffer);
#endif
}

