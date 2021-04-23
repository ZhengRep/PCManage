#pragma once
#include "Ntdll.h"
#include "StringHelper_T.h"



template <class T>
struct _LIST_ENTRY_T
{
	T Flink;
	T Blink;
};

template<class T>
struct _CURDIR_T
{
	_UNICODE_STRING_T<T> DosPath;
	T Handle;
};

template<class T>
struct _RTL_DRIVE_LETTER_CURDIR_T
{

	WORD Flags;
	WORD Length;
	ULONG TimeStamp;
	_UNICODE_STRING_T<T> DosPath;

};

template <class T>
struct _RTL_USER_PROCESS_PARAMETERS_T
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	T     ConsoleHandle;
	ULONG ConsoleFlags;
	T     StandardInput;
	T     StandardOutput;
	T     StandardError;
	_CURDIR_T<T> CurrentDirectory;
	_UNICODE_STRING_T<T> DllPath;
	_UNICODE_STRING_T<T> ImagePathName;
	_UNICODE_STRING_T<T> CommandLine;
	T     Environment;
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	_UNICODE_STRING_T<T> WindowTitle;
	_UNICODE_STRING_T<T> DesktopInfo;
	_UNICODE_STRING_T<T> ShellInfo;
	_UNICODE_STRING_T<T> RuntimeData;
	_RTL_DRIVE_LETTER_CURDIR_T<T> CurrentDirectores[32];
	T     EnvironmentSize;
	T     EnvironmentVersion;
};
typedef _RTL_USER_PROCESS_PARAMETERS_T<DWORD> RTL_USER_PROCESS_PARAMETERS32;
typedef _RTL_USER_PROCESS_PARAMETERS_T<DWORD64> RTL_USER_PROCESS_PARAMETERS64;

template <class T, class NGF, int A>
struct _PEB_
{
	union
	{
		struct
		{
			BYTE InheritedAddressSpace;
			BYTE ReadImageFileExecOptions;
			BYTE BeingDebugged;
			BYTE BitField;
		};
		T dummy01;
	};
	T Mutant;
	T ImageBaseAddress;
	T Ldr;
	T ProcessParameters;
	T SubSystemData;
	T ProcessHeap;
	T FastPebLock;
	T AtlThunkSListPtr;
	T IFEOKey;
	T CrossProcessFlags;
	T UserSharedInfoPtr;
	DWORD SystemReserved;
	DWORD AtlThunkSListPtr32;
	T ApiSetMap;
	T TlsExpansionCounter;
	T TlsBitmap;
	DWORD TlsBitmapBits[2];
	T ReadOnlySharedMemoryBase;
	T HotpatchInformation;
	T ReadOnlyStaticServerData;
	T AnsiCodePageData;
	T OemCodePageData;
	T UnicodeCaseTableData;
	DWORD NumberOfProcessors;
	union
	{
		DWORD NtGlobalFlag;
		NGF dummy02;
	};
	LARGE_INTEGER CriticalSectionTimeout;
	T HeapSegmentReserve;
	T HeapSegmentCommit;
	T HeapDeCommitTotalFreeThreshold;
	T HeapDeCommitFreeBlockThreshold;
	DWORD NumberOfHeaps;
	DWORD MaximumNumberOfHeaps;
	T ProcessHeaps;
	T GdiSharedHandleTable;
	T ProcessStarterHelper;
	T GdiDCAttributeList;
	T LoaderLock;
	DWORD OSMajorVersion;
	DWORD OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	DWORD OSPlatformId;
	DWORD ImageSubsystem;
	DWORD ImageSubsystemMajorVersion;
	T ImageSubsystemMinorVersion;
	T ActiveProcessAffinityMask;
	T GdiHandleBuffer[A];
	T PostProcessInitRoutine;
	T TlsExpansionBitmap;
	DWORD TlsExpansionBitmapBits[32];
	T SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	T pShimData;
	T AppCompatInfo;
	_UNICODE_STRING_T<T> CSDVersion;
	T ActivationContextData;
	T ProcessAssemblyStorageMap;
	T SystemDefaultActivationContextData;
	T SystemAssemblyStorageMap;
	T MinimumStackCommit;
	T FlsCallback;
	_LIST_ENTRY_T<T> FlsListHead;
	T FlsBitmap;
	DWORD FlsBitmapBits[4];
	T FlsHighIndex;
	T WerRegistrationData;
	T WerShipAssertPtr;
	T pContextData;
	T pImageHeaderHash;
	T TracingFlags;
};


typedef _PEB_<DWORD, DWORD64, 34> PEB32;
typedef _PEB_<DWORD64, DWORD, 30> PEB64;

template<typename T>
struct _PEB_T
{
	typedef typename std::conditional<std::is_same<T, DWORD>::value, PEB32, PEB64>::type type;
};


template<typename T>
struct _PROCESS_BASIC_INFORMATION_T
{
	NTSTATUS ExitStatus;
	ULONG    Reserved0;
	T	     PebBaseAddress;
	T	     AffinityMask;
	LONG	 BasePriority;
	ULONG	 Reserved1;
	T	     UniqueProcessId;
	T	     InheritedFromUniqueProcessId;
};


template <class T>
struct _PEB_LDR_DATA_T
{
	DWORD Length;
	DWORD Initialized;
	T SsHandle;
	_LIST_ENTRY_T<T> InLoadOrderModuleList;
	_LIST_ENTRY_T<T> InMemoryOrderModuleList;
	_LIST_ENTRY_T<T> InInitializationOrderModuleList;
	T EntryInProgress;
	DWORD ShutdownInProgress;
	T ShutdownThreadId;

};
typedef _PEB_LDR_DATA_T<DWORD> PEB_LDR_DATA32;
typedef _PEB_LDR_DATA_T<DWORD64> PEB_LDR_DATA64;


template <class T>
struct _LDR_DATA_TABLE_ENTRY_T
{
	_LIST_ENTRY_T<T> InLoadOrderLinks;
	_LIST_ENTRY_T<T> InMemoryOrderLinks;
	_LIST_ENTRY_T<T> InInitializationOrderLinks;
	T DllBase;
	T EntryPoint;
	union
	{
		DWORD SizeOfImage;
		T dummy01;
	};
	_UNICODE_STRING_T<T> FullDllName;
	_UNICODE_STRING_T<T> BaseDllName;
	DWORD Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		_LIST_ENTRY_T<T> HashLinks;
		struct
		{
			T SectionPointer;
			T CheckSum;
		};
	};
	union
	{
		T LoadedImports;
		DWORD TimeDateStamp;
	};
	T EntryPointActivationContext;
	T PatchInformation;
	_LIST_ENTRY_T<T> ForwarderLinks;
	_LIST_ENTRY_T<T> ServiceTagLinks;
	_LIST_ENTRY_T<T> StaticLinks;
	T ContextInformation;
	T OriginalBase;
	_LARGE_INTEGER LoadTime;
};
typedef _LDR_DATA_TABLE_ENTRY_T<DWORD> LDR_DATA_TABLE_ENTRY32;
typedef _LDR_DATA_TABLE_ENTRY_T<DWORD64> LDR_DATA_TABLE_ENTRY64;

