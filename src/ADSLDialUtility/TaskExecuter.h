#pragma once
#include "RemoteTaskDefs.h"

class CTaskExecuter
{
public:
	CTaskExecuter();
	~CTaskExecuter();
	BOOL ExecuteTask(CSmartPtr<RemoteTask> task);
protected:
	BOOL ExecuteCommandLine(CString cmdLine, DWORD & exitCode);
};

