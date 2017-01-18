#include "stdafx.h"
#include "IEControl.h"


CIEControl::CIEControl()
{
}


CIEControl::~CIEControl()
{
}

void CIEControl::ClearCookies()
{
// 		Cookies
// 		RunDll32.exe InetCpl.cpl, ClearMyTracksByProcess 2
	ShellExecute(NULL, _T("open"), _T("rundll32.exe"), _T("InetCpl.cpl,ClearMyTracksByProcess 2"), NULL, SW_SHOWNORMAL);
}

void CIEControl::ClearTmpFiles()
{
// 	Temporary Internet Files(InternetÁÙÊ±ÎÄ¼þ)
// 	RunDll32.exe InetCpl.cpl, ClearMyTracksByProcess 8
	ShellExecute(NULL, _T("open"), _T("rundll32.exe"), _T("InetCpl.cpl,ClearMyTracksByProcess 8"), NULL, SW_SHOWNORMAL);
}
