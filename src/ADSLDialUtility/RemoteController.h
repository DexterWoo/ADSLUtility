#pragma once
#include <afxmt.h>
#include "HttpClient.h"
#include "RemoteCmdParser.h"
#include "TaskExecuter.h"

class CRemoteController
{
public:
	CRemoteController();
	~CRemoteController();

	void StartBGWork();
	void StopBGWork();
	
protected:
	void WorkThread();
	CSmartPtr<RemoteTask> GetOneTask();
private:
	CHttpClient client;
	CRemoteCmdParser cmdParser;
	CTaskExecuter taskExecuter;
	CString m_strClientId;
	CString m_strCmdUrl;
	bool			 m_xStop;
	CEvent			m_Event;
	CEvent		m_ExitEvent;
};

