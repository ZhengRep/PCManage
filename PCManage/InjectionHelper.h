#pragma once
#include<iostream>
#include<tchar.h>
#include<windows.h>
#include "ProcessHelper.h"
#include "Ntdll.h"
using namespace std;

BOOL FaInjectModule1(const TCHAR* ModulePath, HANDLE ProcessIdentify);