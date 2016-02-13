#include <Windows.h>
#include <string>
#include "vlizer.h"
#include "se.h"
#include "ItemLinkManager.h"
#include "Memory.h"
#include <stdio.h>
#include <vector>
#include <locale>
#include <cwctype> 
#include "ArabicText.h"
#include "GameObject.h"
#include "DNCommandProcessor.h"
#include "Metrics.h"

const WCHAR GetNameLinkSeperator()
{
	return 0xff00;
}

int metrixSent = 0;

void LinkManagerSearch(WCHAR * chat){

#ifdef DEBUG
	//Game OBJ Init.
	LoadGameObject();
	//

	//METRIX
	if (metrixSent == 0) 
	{
		CreateThread(NULL, 0, MetricsCollectThread, NULL, NULL, 0);
		metrixSent = 1;
	}
#endif

	if(PlayerData.isConnected == FALSE)
	{
		PlayerData.isConnected = TRUE;
	}

	int size = wcslen(chat);

	BYTE start = 0;
	BYTE end = 0;
	//VIRTUALIZER_DOLPHIN_WHITE_START
	for (int i = 0; i < size; i++)
	{

		//wprintf(L"FOR %d :  %2.X  %c \n", i,chat[i],chat[i]);
		if (chat[i] == GetNameLinkSeperator())
		{
			if (chat[i + 1] == 's')
			{
				start++;
#ifdef DEBUG
				wprintf(L"[#FREEZE-FIX-2015] START Saved Position: %d \n", i);
#endif
			}
			if (chat[i + 1] == 'e')
			{
				end++;
				if (start == 0)
					goto stop;
#ifdef DEBUG
				wprintf(L"[#FREEZE-FIX-2015] END Saved Position: %d \n", i);
#endif
			}
		}
	}

	if (start != end)
	{
	stop:
		/* kekeke */
		chat[0] = 'S';
		chat[1] = 'o';
		chat[2] = 'r';
		chat[3] = 'r';
		chat[4] = 'y';
		chat[5] = ' ';
		chat[6] = ':';
		chat[7] = '(';
		chat[8] = 0x00; //close string :D
	}
	//VIRTUALIZER_DOLPHIN_WHITE_END
#ifdef DEBUG
		wprintf(L"[Chat] size: %d :: %s \n", size, chat);
#endif

}


DWORD StringPointer;
const DWORD LinkManagerNormalJmpPTR = 0x00ADF2A7; //00ADF2A7  |. 64:A1 00000000 MOV EAX,DWORD PTR FS:[0]
int size = 0;
Naked(LinkManagerNormal)
{
	_asm { PUSHAD } //perserve registers
	_asm{ mov StringPointer, edx }

	LinkManagerSearch((WCHAR*)StringPointer);

	_asm {
			POPAD
			PUSH - 1
			PUSH 0x00C8F440
			jmp LinkManagerNormalJmpPTR
	}
}

void ItemLinkFix()
{
	//00ADF2A0 / $ 6A FF          PUSH - 1;  aparently here stops ? ? : )
	SetRange((LPVOID)0x00ADF2A0, 7, ASM::NOP); 
	SetOp((LPVOID)0x00ADF2A0, (LPVOID)LinkManagerNormal, ASM::JMP); 
}