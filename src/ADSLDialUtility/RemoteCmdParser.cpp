#include "stdafx.h"
#include "RemoteCmdParser.h"
#include "json11.hpp"
CRemoteCmdParser::CRemoteCmdParser()
{
}


CRemoteCmdParser::~CRemoteCmdParser()
{
}

CSmartPtr<RemoteTask> CRemoteCmdParser::ParseFromJson(std::string strJson)
{
	CSmartPtr<RemoteTask> task;
	std::string strErr;
	json11::Json jsonObj = json11::Json::parse(strJson, strErr);
	if (strErr.empty())
	{
		task = new RemoteTask();
		task->strCmd = jsonObj["body"]["cmd"].string_value();
		task->strTaskId = jsonObj["body"]["taskId"].string_value();
		if (task->strTaskId == "")
		{
			task->strTaskId = std::to_string(jsonObj["body"]["taskId"].int_value());
		}
		task->strDesc = jsonObj["body"]["desc"].string_value();

		if (task->strCmd == CmdExeCmd)
		{
			task->cmdData = new CmdData();
			task->cmdData->strCmd = jsonObj["body"]["data"]["cmd"].string_value();
		}
		else if (task->strCmd == CmdDownload)
		{
			task->downloadData = new DownloadData();
			task->downloadData->strUrl = jsonObj["body"]["data"]["url"].string_value();
			task->downloadData->strLocalPath = jsonObj["body"]["data"]["localpath"].string_value();
		}
		else if (task->strCmd == CmdSleep)
		{
			task->sleepData = new SleepData;
			task->sleepData->iSleepTime = jsonObj["body"]["data"]["sleeptime"].int_value();
		}
	}

	return task;
}
