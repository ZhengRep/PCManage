#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include<strsafe.h>
using namespace std;



typedef struct UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;