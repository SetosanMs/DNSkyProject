#include <windows.h>

#define MAX_DUMP_OFFSETS 94
#define MAX_DUMP_SIZE 32
#define MAX_PROC_NAME 32
#if defined(CHN_MSTERE)
#define MAX_PROCESS_DUMP 103
#else
#define MAX_PROCESS_DUMP 107
#endif

typedef struct ANITHACK_PROCDUMP {
	unsigned int m_aOffset;
	unsigned char m_aMemDump[MAX_DUMP_SIZE];
	char m_ProcName[MAX_PROC_NAME];
} *PANITHACK_PROCDUMP;

extern ANITHACK_PROCDUMP g_ProcessesDumps[MAX_PROCESS_DUMP];

void SystemProcessesScan();
bool ScanProcessMemory(HANDLE hProcess);
void Antihack_MainThread();
void MemoryScanner();
