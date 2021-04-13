#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

BOOL FaEnableDebugPrivilege(BOOL IsEnable);
BOOL FaEnableLoadDriverPrivilege(BOOL IsEnable);