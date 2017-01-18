#pragma once
#include "Dialupass.h"
#include "RasClient.h"
#include <ras.h>
#include <vector>
using namespace std;

//自定义消息，通知UI状态改变 
#define WM_DIALSTATE_CHANGED  (WM_USER+0x89)
void CALLBACK RasDialFunc(
	_In_ UINT         unMsg,
	_In_ RASCONNSTATE rasconnstate,
	_In_ DWORD        dwError
	);


class CRasControl
{
public:
	CRasControl();
	~CRasControl();
	static CWnd* pWnd;
public:
	UINT GetEntryCount();
	void Dial(int index);
	CString GetEntryName(int index);
	CString GetEntryUserName(int index);
	CString GetEntryPassword(int index);
	CString GetCurIP();
	void HangUp(int index);

protected:
	void InitEntrylist();


private:
	CDialupass* m_dialPass;
	CRasClient* m_rasClient;
	vector<LPRASDIALPARAMS> m_entryList;
};

