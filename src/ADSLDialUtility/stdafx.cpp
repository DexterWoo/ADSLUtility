
// stdafx.cpp : 只包括标准包含文件的源文件
// ADSLDialUtility.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


CString Int2CString(UINT value)
{
	CString str;
	str.Format(STR(%d), value);
	return str;
}
