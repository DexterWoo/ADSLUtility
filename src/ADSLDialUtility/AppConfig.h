#pragma once


class CAppConfig
{
public:
	CAppConfig();
	~CAppConfig();

public:
	UINT ConnectionIndex;
	UINT DeleteCookies;
	UINT TimeToDeleteCookies;
	UINT DeleteIETmp;
	UINT TimeToDeleteIETmp;
	UINT TimeConnection;
	UINT TimeHangup;
	UINT TimedConnect;
	UINT BootStart;
	UINT IPRepeat;
private:
	CString strFileName;
};

