#pragma once
#include "SmartPtr.h"
#include <string>
#define CmdRedial			"ppop"
#define CmdExeCmd		"execmd"
#define CmdDownload		"download"
#define CmdSleep		"sleep"

#define WM_REDIAL	(WM_USER+0x250)
struct CmdData
{
	std::string strCmd;
};

struct DownloadData
{
	std::string strUrl;
	std::string strLocalPath;
};

struct SleepData
{
	int iSleepTime;
};

struct RemoteTask
{
	std::string strTaskId;
	std::string strCmd;
	std::string strDesc;
	CSmartPtr<CmdData> cmdData;
	CSmartPtr<DownloadData> downloadData;
	CSmartPtr<SleepData> sleepData;
};

