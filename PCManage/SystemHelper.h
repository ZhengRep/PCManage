#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;


//����һ��ϵͳ�汾ö�ٵĴ�С4���ֽ�
typedef enum _OS_VERSION_ {
	WINDOWS_UNKNOW,
	WINDOWS_7,
	WINDOWS_10
}OS_VERSION;

OS_VERSION FaGetOsVersion();

BOOL FaSetStringToClipboard(CString ParameterData);//���ַ���ת��ճ����
void FaLocationExplorer(CString ImagePath);