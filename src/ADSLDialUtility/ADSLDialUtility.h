
// ADSLDialUtility.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CADSLDialUtilityApp: 
// �йش����ʵ�֣������ ADSLDialUtility.cpp
//

class CADSLDialUtilityApp : public CWinApp
{
public:
	CADSLDialUtilityApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CADSLDialUtilityApp theApp;