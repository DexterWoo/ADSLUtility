#pragma once
#include "RemoteTaskDefs.h"

class CRemoteCmdParser
{
public:
	CRemoteCmdParser();
	~CRemoteCmdParser();
	CSmartPtr<RemoteTask> ParseFromJson(std::string strJson);
};

