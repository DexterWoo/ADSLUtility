
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ADSLDialUtility.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


CString Int2CString(UINT value)
{
	CString str;
	str.Format(STR(%d), value);
	return str;
}
