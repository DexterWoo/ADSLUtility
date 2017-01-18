#include "stdafx.h"
#include "RemoteController.h"
#include "Log.h"
#include "ThreadPool.h"
#define  UrlDef _T("http://112.124.0.127:8880/bu88qq/getCmd?clientId=%s")
#define  ReportUrlDef  _T("http://112.124.0.127:8880/bu88qq/report?taskId=%s&clientId=%s&resultStatus=%d")
CRemoteController::CRemoteController()
{
	m_xStop = false;
	CStdioFile file;
	TRY 
	{
		if (file.Open(_T("C:\\clientid.txt"), CFile::modeRead))
		{
			file.ReadString(m_strClientId);
			m_strCmdUrl.Format(UrlDef, m_strClientId);
			file.Close();
		}
	}
	CATCH (CException, e)
	{
		TCHAR szError[256] = { 0 };
		e->GetErrorMessage(szError, 255);
		Log.error(_T("HttpGet"), _T("Error:%s"), szError);
	}
	END_CATCH
	
}


CRemoteController::~CRemoteController()
{
	StopBGWork();
}

void CRemoteController::StartBGWork()
{
	try
	{
		CThreadPool::QueueUserWorkItem(&CRemoteController::WorkThread, this);
	}
	catch (DWORD lastErr)
	{
		//do nothing
		lastErr = lastErr;
	}
}

void CRemoteController::StopBGWork()
{
	m_xStop = true;
	m_Event.SetEvent();
	m_ExitEvent.Lock(30 * 1000);
}

void CRemoteController::WorkThread()
{
	while (!m_xStop)
	{
		CSmartPtr<RemoteTask> task = GetOneTask();
		if (task != NULL)
		{
			BOOL bRet = taskExecuter.ExecuteTask(task);
			//do a report.
			CString strReport;
			strReport.Format(ReportUrlDef, CString(task->strTaskId.c_str()), m_strClientId, bRet ? 4 : 3);
			client.HttpGet(strReport);
		}
		m_Event.Lock(2*1000);
	}
	m_ExitEvent.SetEvent();
}

CSmartPtr<RemoteTask> CRemoteController::GetOneTask()
{
	return cmdParser.ParseFromJson(client.HttpGet(m_strCmdUrl));
}
