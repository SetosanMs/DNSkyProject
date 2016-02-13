#include <Windows.h>
#include <stdio.h>
#include "doutputstring.h"
#include "vlizer.h"
#include "Security.h"
#include "Guard.h"


int threadFailCount = 0;
DWORD LastThreadStarted = 0;


void (WINAPI *dOutputDebugString)(LPCTSTR lpOutputString) = NULL;
void WINAPI myOutputDebugString(LPCTSTR lpOutputString);

void WINAPI myOutputDebugString(LPCTSTR lpOutputString)
{

	//VIRTUALIZER_DOLPHIN_WHITE_START
	if (tickCheck + 3000 < GetTickCount())
	{
		if (threadFailCount == 3)
		{
			s.crash();
		}

		if (LastThreadStarted + 5000 < GetTickCount()){
			threadFailCount++;
			//VIRTUALIZER_DOLPHIN_WHITE_END
			InitGuard();
			LastThreadStarted = GetTickCount();
		}

		//	MessageBoxA(NULL, "CPU TICK MISSMATCH, FORCE CLOSE!", "OK", MB_OK);

		//
	}

#ifdef DEBUG_
	printf("%s", lpOutputString);
#endif
	//return dOutputDebugString(lpOutputString);
}