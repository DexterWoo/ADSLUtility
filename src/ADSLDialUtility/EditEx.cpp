// EditEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADSLDialUtility.h"
#include "EditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


CString CEditEx::GetText()
{
	CString retWindowText;
	GetWindowText(retWindowText);
	
	return retWindowText;
}

unsigned long CEditEx::GetInt()
{
	CString retWindowText;
	GetWindowText(retWindowText);
	unsigned long retVal = _ttol(retWindowText);
	return retVal;
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
END_MESSAGE_MAP()



// CEditEx ��Ϣ�������


