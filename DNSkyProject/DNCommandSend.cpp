#include <Windows.h>
#include "DNCommandProcessor.h"
#include "Memory.h"


bool commandNulled = false;

const DWORD JMPBACK = 0x00401066; /// $ 81EC 18040000  SUB ESP, 0x418
DWORD PointerChat = NULL;
Naked(Command) ///0x0B 0x01
{
	_asm{
		PUSHAD
		mov PointerChat,ECX
	}
		
	ParseCommand((WCHAR*)PointerChat);
	if (commandNulled == true)
	{
		commandNulled = false;
		_asm{
			retn; //poate retn 3
		}
	}


	_asm{
		POPAD
		SUB ESP, 0x418
		JMP JMPBACK
	}

}


const DWORD JMPBACKPM = 0x00805519;
DWORD PointerChatPM = NULL;
Naked(CommandPM) ///0x0B 0x02 PM
{
	_asm{
		PUSHAD
		mov PointerChatPM,ECX
	}

	ParseCommand((WCHAR*)PointerChatPM);

	/*00805514   83C6 04          ADD ESI,0x4
	00805517   8BC6             MOV EAX,ESI
	jmp
	00805519   50               PUSH EAX
	
	*/
	_asm{
		POPAD
		ADD ESI, 0x4
		MOV EAX, ESI
		JMP JMPBACKPM
	}

}

void LoadCommandProcessor()
{
	SetRange((LPVOID)0x00401060, 6, ASM::NOP);
	SetOp((LPVOID)0x00401060, (LPVOID)Command, ASM::JMP);


	SetRange((LPVOID)0x00805514, 5, ASM::NOP);
	SetOp((LPVOID)0x00805514, (LPVOID)CommandPM, ASM::JMP);
}
