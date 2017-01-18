#include "stdafx.h"
#include "AppConfig.h"


CAppConfig::CAppConfig()
{
	TCHAR buffer[256];
	GetModuleFileName(NULL, buffer, 256);
	strFileName = buffer;
	strFileName.Replace(STR(.exe), STR(.cfg));
	/*UINT ConnectionIndex;
	CHECKBOXSTATES DeleteCookies;
	UINT TimeToDeleteCookies;
	CHECKBOXSTATES DeleteIETmp;
	UINT TimeToDeleteIETmp;
	UINT TimeConnection;
	UINT TimeHangup;
	CHECKBOXSTATES TimedConnect;
	CHECKBOXSTATES BootStart;
	CHECKBOXSTATES IPRepeat;*/
	ConnectionIndex = GetPrivateProfileInt(STR(AppSettings), STR(ConnectionIndex), 0, strFileName);
	DeleteCookies = GetPrivateProfileInt(STR(AppSettings), STR(DeleteCookies), BST_UNCHECKED, strFileName);
	TimeToDeleteCookies = GetPrivateProfileInt(STR(AppSettings), STR(TimeToDeleteCookies), 600, strFileName);
	DeleteIETmp = GetPrivateProfileInt(STR(AppSettings), STR(DeleteIETmp), BST_UNCHECKED, strFileName);
	TimeToDeleteIETmp = GetPrivateProfileInt(STR(AppSettings), STR(TimeToDeleteIETmp), 1800, strFileName);
	TimeConnection = GetPrivateProfileInt(STR(AppSettings), STR(TimeConnection), 30, strFileName);
	TimeHangup = GetPrivateProfileInt(STR(AppSettings), STR(TimeHangup), 7, strFileName);
	TimedConnect = GetPrivateProfileInt(STR(AppSettings), STR(TimedConnect), BST_UNCHECKED, strFileName);
	BootStart = GetPrivateProfileInt(STR(AppSettings), STR(BootStart), BST_UNCHECKED, strFileName);
	IPRepeat = GetPrivateProfileInt(STR(AppSettings), STR(IPRepeat), BST_UNCHECKED, strFileName);
}


CAppConfig::~CAppConfig()
{
	WritePrivateProfileString(STR(AppSettings), STR(ConnectionIndex), Int2CString(ConnectionIndex), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(DeleteCookies), Int2CString(DeleteCookies), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(TimeToDeleteCookies), Int2CString(TimeToDeleteCookies), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(DeleteIETmp), Int2CString(DeleteIETmp), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(TimeToDeleteIETmp), Int2CString(TimeToDeleteIETmp), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(TimeConnection), Int2CString(TimeConnection), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(TimeHangup), Int2CString(TimeHangup), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(TimedConnect), Int2CString(TimedConnect), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(BootStart), Int2CString(BootStart), strFileName);
	WritePrivateProfileString(STR(AppSettings), STR(IPRepeat), Int2CString(IPRepeat), strFileName);
}
