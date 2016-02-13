#include <Windows.h>
#include <vector>
#include "vlizer.h"
#include "dreadfile.h"
#include "se.h"
#include "Extras.h"
#include "Crypto.h"
#include "Misc.h"
#include "PackingSystem.h"

/* ReadFile */
BOOL (WINAPI *dReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped) = NULL;
BOOL WINAPI myReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped);


BOOL WINAPI myReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD  nNumberOfBytesToRead, LPDWORD lpNumberOfByte, LPOVERLAPPED lpOverlapped)
{
#ifdef DEBUG
//	TCHAR Path[255];
//	GetFinalPathNameByHandle(hFile, Path, 255, VOLUME_NAME_NT);
#endif
	//VIRTUALIZER_DOLPHIN_WHITE_START

	//DECODE HEADER!
	if (Packing.isHeader(nNumberOfBytesToRead))
	{
		BOOL result = dReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfByte, lpOverlapped);
		
		char stampila[0x20]; //32baiti baito :P
		strncpy(stampila, ((const char*)lpBuffer), 0x1F);
		stampila[0x1F] = '\0';

		if (Packing.AddFile(hFile, Packing.CalculateVersion(stampila)))
		{
			Packing.DecryptHeader((BYTE*)lpBuffer);
		}
		return result;
	}


	//DECODE FILELIST!! done.
	if (Packing.GetHandle(hFile) && Packing.isFooter(nNumberOfBytesToRead))
	{
		BOOL result = dReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfByte, lpOverlapped);

		Packing.DecryptFooter((BYTE*)lpBuffer, nNumberOfBytesToRead);

		return result;
	}

	//v1.1++ addon : Data decryption was added. 
	if (Packing.GetHandle(hFile) && Packing.GetVersionByHandle(hFile) > PackVersion::RALUKAT_v10) //doar pt ver 1.1++
	{
		BOOL result = dReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfByte, lpOverlapped);
		Packing.DecryptData((BYTE*)lpBuffer, nNumberOfBytesToRead);
		return result;
	}

	//VIRTUALIZER_DOLPHIN_WHITE_END
		return dReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfByte, lpOverlapped);
}