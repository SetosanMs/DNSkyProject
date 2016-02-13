#include <Windows.h>
#include <stdio.h>
#include "Memory.h"
#include "EmitterEffect.h"
#include "vlizer.h"
#include <VMProtectSDK.h>

void AnnounceEmmiter()
{
	printf("Loading EffectEmmiter 0xD\n");
}


const DWORD Call1 = 0x00BE0FD0;
const DWORD Call2 = 0x00BE1050;
const DWORD Call3 = 0x00AB98C0;
const DWORD Call4 = 0x00BDF5A0;
const DWORD Call5 = 0x00C450D1;
Naked(EmitterEffect)
{

	//VMProtectBeginVirtualization("EffectEmiter");

	_asm{
		SUB ESP, 0x114
		MOV EAX, DWORD PTR DS : [0xE7E490]
		XOR EAX, ESP
		MOV DWORD PTR SS : [ESP + 0x110], EAX
		MOV EAX, DWORD PTR SS : [ESP + 0x11C]
		SUB EAX, 0xA
		PUSH EBX
		MOV EBX, DWORD PTR SS : [ESP + 0x11C]
		PUSH EBP
		PUSH ESI
		PUSH EDI
		MOV ESI, ECX
		JE VersionA //
		SUB EAX, 0x1
		JE VersionB //
		SUB EAX, 0x1
		JE VersionC //
		SUB EAX, 0x1
		JE VersionD //
		PUSH 0x37
		PUSH 0x00DDC6F0//;  UNICODE ".\EtBillboardEffectEmitter.cpp"
		PUSH 0x00DDC730//;  UNICODE "0 && "Invalid EmitterVersion""
		CALL DWORD PTR DS : [0xE80E70]//;  dragon.00AC0340
		MOV EAX, DWORD PTR DS : [ESI]
		MOV EDX, DWORD PTR DS : [EAX + 0x1C]
		ADD ESP, 0xC
		PUSH 0x1
		PUSH 0x400
		MOV ECX, ESI
		CALL EDX
		JMP Continue2

VersionD:
#ifdef DEBUG
		PUSHAD
		CALL AnnounceEmmiter
		POPAD
#endif
		MOV EAX, DWORD PTR DS : [ESI]//;  Case D of switch
		MOV EDX, DWORD PTR DS : [EAX + 0x20]
		PUSH 0x19C
		PUSH EBX
		CALL EDX
		MOV EAX, DWORD PTR DS : [ESI]
		MOV EDX, DWORD PTR DS : [EAX + 0x1C]
		PUSH 0x1
		PUSH 0x264
		MOV ECX, ESI
		CALL EDX
		JMP Continue2

VersionC:
		MOV EAX, DWORD PTR DS : [ESI]//;  Case C of switch 00BDF48B
		MOV EDX, DWORD PTR DS : [EAX + 0x20]
		PUSH 0x198
		PUSH EBX
		CALL EDX
		MOV EAX, DWORD PTR DS : [ESI]
		MOV EDX, DWORD PTR DS : [EAX + 0x1C]
		PUSH 0x1
		PUSH 0x268
		MOV ECX, ESI
		CALL EDX
		JMP Continue2

VersionB:
		MOV EAX, DWORD PTR DS : [ESI]//;  Case B of switch 00BDF48B
		MOV EDX, DWORD PTR DS : [EAX + 0x20]
		PUSH 0x190
		PUSH EBX
		CALL EDX
		PUSH 0x1
		PUSH 0x270
		JMP Continue

VersionA:
		MOV EAX, DWORD PTR DS : [ESI]//;  Case A of switch 00BDF48B
		MOV EDX, DWORD PTR DS : [EAX + 0x20]
		PUSH 0x16C
		PUSH EBX
		CALL EDX
		PUSH 0x1
		PUSH 0x294
		JMP Continue

Continue:
		MOV EAX, DWORD PTR DS : [ESI]
		MOV EDX, DWORD PTR DS : [EAX + 0x1C]
		MOV ECX, ESI
		CALL EDX
		MOV EAX, DWORD PTR DS : [EBX]
		MOV DWORD PTR DS : [EBX + 0x190], EAX
		MOV DWORD PTR DS : [EBX + 0x194], EAX
		JMP Continue2

Continue2:
		LEA EDI, DWORD PTR DS : [EBX + 0x208]
		MOV EBP, 0x3
		LEA ESP, DWORD PTR SS : [ESP]
		JMP Loop1

		//loop
Loop1:
		MOV EAX, DWORD PTR DS : [EDI]
		CALL Call1	
		MOV EAX, DWORD PTR DS : [EDI]
		PUSH ESI
		PUSH EAX
		CALL Call2 
		ADD EDI, 0x4
		SUB EBP, 0x1
		JNZ Loop1 //

		LEA EDX, DWORD PTR DS : [EBX + 0x64]
		MOV EAX, EDX
		LEA ESI, DWORD PTR DS : [EAX + 0x1]

Loop2:
		MOV CL, BYTE PTR DS : [EAX]
		ADD EAX, 0x1
		TEST CL, CL
		JNZ Loop2 //

		SUB EAX, ESI
		JE Finish //
		LEA EDI, DWORD PTR SS : [ESP + 0x14]
		CALL Call3 //3
		MOV ECX, EDI
		PUSH ECX
		MOV EAX, EBX
		CALL Call4 //4
		JMP Finish

Finish:
		MOV ECX, DWORD PTR SS : [ESP + 0x120]
		POP EDI
		POP ESI
		POP EBP
		POP EBX
		XOR ECX, ESP
		CALL Call5 //5
		ADD ESP, 0x114
		RETN 0x8
	}
	//VMProtectEnd();
}


void EmitterVersionFix()
{
	SetRange((LPVOID)0x00BDF470, 0x1F, ASM::NOP); // 00BDF470 / $ 81EC 14010000  SUB ESP, 0x114
	SetOp((LPVOID)0x00BDF470, (LPVOID)EmitterEffect, ASM::JMP);
}