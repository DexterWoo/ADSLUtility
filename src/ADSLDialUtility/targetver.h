#pragma once

// ���� SDKDDKVer.h ��������߰汾�Ŀ��� Windows ƽ̨��

// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
// WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��
// We only support Windows XP and later because we use L2TP/IPSec PSK and that is not supported
// on lower versions.
#define _WIN32_WINNT 0x0501 
#include <SDKDDKVer.h>
