#include "stdafx.h"
#include "TaskExecuter.h"
#include "HttpClient.h"

CTaskExecuter::CTaskExecuter()
{
}


CTaskExecuter::~CTaskExecuter()
{
}

BOOL CTaskExecuter::ExecuteTask(CSmartPtr<RemoteTask> task)
{
	BOOL bRet = TRUE;
	
	if (task->strCmd == CmdRedial)
	{
		//Send A Message to MainGUI
		SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_REDIAL, 0, 0);
	}
	else if (task->strCmd == CmdExeCmd)
	{
		DWORD dwRet = 0;
		bRet = ExecuteCommandLine(CString(task->cmdData->strCmd.c_str()), dwRet);
	}
	else if (task->strCmd == CmdDownload)
	{
		CHttpClient client;
		std::string strRet = client.HttpGet(CString(task->downloadData->strUrl.c_str()), CString(task->downloadData->strLocalPath.c_str()));
		bRet = !strRet.empty();
	}
	else if (task->strCmd == CmdSleep)
	{
		Sleep(task->sleepData->iSleepTime * 1000);
	}
	
	
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// Executes the given command using CreateProcess() and WaitForSingleObject().
// Returns FALSE if the command could not be executed or if the exit code could not be determined.
BOOL CTaskExecuter::ExecuteCommandLine(CString cmdLine, DWORD & exitCode)
{
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = cmdLine.GetLength() + 50;


	// Create the process
	BOOL result = CreateProcess(NULL, cmdLine.GetBuffer(nStrBuffer),
		NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
		NULL, NULL, &startupInfo, &processInformation);
	cmdLine.ReleaseBuffer();


	if (!result)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		// Display the error
		CString strError = (LPTSTR)lpMsgBuf;
		TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

		// Free resources created by the system
		LocalFree(lpMsgBuf);

		// We failed.
		return FALSE;
	}
	else
	{
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		// Get the exit code.
		result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

		// Close the handles.
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (!result)
		{
			// Could not get exit code.
			TRACE(_T("Executed command but couldn't get exit code.\nCommand=%s\n"), cmdLine);
			return FALSE;
		}


		// We succeeded.
		return TRUE;
	}
}