#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

typedef _Return_type_success_(return >= 0) long NTSTATUS, * PNTSTATUS;

#ifndef NT_SUCCESS
#define NT_SUCCESS(StatCode)  ((NTSTATUS)(StatCode) >= 0)
#endif

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

bool FaCloseHandle(HANDLE HandleValue);
