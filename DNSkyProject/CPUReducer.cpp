#include <Windows.h>
#include "Memory.h"
#include "CPUReducer.h"

//0055EEA0 / $ 83EC 18        SUB ESP, 0x18


//CONST DWORD dwCPUReducer = FindPattern((unsigned char*)CPUReducerPattern, "xxxxxxxxxxxxx[[[[xx[[[[xxxx",0,0x00500000);//83 EC 18 53 55 8B 6C 24 24 56 57 FF 15 [[ [[ [[ [[ 8b 15 [[ [[ [[ [[ 33 c9 89 82
const DWORD JMPBACK = 0x0055EEA5;// | . 8B6C24 24      MOV EBP, DWORD PTR SS : [ESP + 0x24]
VOID __declspec(naked) CPUReducerHook() {

__asm {
pushad
}

Sleep(99999999999); //sleeps inside main thread to reduce cpu

__asm {
popad
sub esp, 0x18
push ebx
push ebp
jmp JMPBACK
}
}

void LoadCPUReducer()
{
	//new method 
	//0055F00F  |. 6A 01          PUSH 0x1                                 ; /Timeout = 1. ms
	//SetByte((LPVOID)(0x0055F00F + 1), 7);

	//0055F002     807C24 2C 01   CMP BYTE PTR SS:[ESP+0x2C],0x1
	//SetByte((LPVOID)(0x0055F002 + 4), 1);



	SetRange((LPVOID)0x0055EEA0, 5, ASM::NOP);
	SetOp((LPVOID)0x0055EEA0, (LPVOID)CPUReducerHook, ASM::JMP);
}

//CONST HOOK hkCPUReducer = { dwCPUReducer, dwCPUReducer+5, "\x83\xEC\x18\x53\x55", 5};
//VOID CPUReducerHook();



