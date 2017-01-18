#pragma once


// CEditEx

class CEditEx : public CEdit
{
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();
	CString GetText();
	unsigned long GetInt();
protected:
	DECLARE_MESSAGE_MAP()
};


