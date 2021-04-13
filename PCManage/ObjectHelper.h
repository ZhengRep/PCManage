#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "Ntdll.h"
#include "StringHelper.h"
using namespace std;

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllTypesInformation,
	ObjectHandleInformation
}OBJECT_INFORMATION_CLASS;


typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	ULONG PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;


typedef
NTSTATUS(NTAPI* LPFN_NTQUERYOBJECT)(IN HANDLE ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID ObjectInformation,
	IN ULONG Length,
	OUT PULONG ResultLength OPTIONAL);




extern LPFN_NTQUERYOBJECT  __NtQueryObject;