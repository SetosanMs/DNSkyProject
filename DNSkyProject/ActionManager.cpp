//By Alin1337
#include <Windows.h>
#include <stdio.h>
#include "Memory.h"
#include "vlizer.h"
#include <VMProtectSDK.h>


#ifdef DEBUG
void Announce()
{
	printf("Loading Act V3!\n");
}
#endif

const DWORD LoadAct1Call = 0x00C11020;
const DWORD ReadStringCall = 0x00ABA3B0;
Naked(ActionManager)
{
	VMProtectBeginVirtualization("ACT v3");

	_asm{
		PUSH ECX
		PUSH EBX
		MOV EBX, DWORD PTR SS : [ESP + 0xC]
		PUSH ESI
		MOV ESI, ECX
		TEST ESI, ESI
		PUSH EDI
		MOV EDI, EAX
		JE END
		MOV EAX, DWORD PTR DS : [ESI + 0x90]
		SUB EAX, 0x1
		JE ACT_V1
		SUB EAX, 0x1
		JE ACT_V2  /*SHORT*/
		SUB EAX, 0x1
		JE ACT_V3
		//
		PUSH 0x228 //;  Default case of switch 00C10F7A
		PUSH 0x00DDE170 // UNICODE ".\EtActionCoreMng.cpp"
		PUSH 0x00DDE1A0
		CALL DWORD PTR DS : [0xE80E70] // dragon.00AC0340
		ADD ESP, 0xC
		POP EDI
		POP ESI
		POP EBX
		POP ECX
		RETN 0x4

ACT_V1:
			PUSH EBX
			MOV EAX, EDI
			CALL LoadAct1Call
			JMP END

ACT_V2:
			CALL ReadStringCall
			LEA ECX, DWORD PTR DS : [ESI + 0x1C]
			CALL ReadStringCall
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4
			LEA ECX, DWORD PTR DS : [ESI + 0x54]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX
			LEA ECX, DWORD PTR DS : [ESI + 0x38]
			CALL ReadStringCall
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4
			LEA ECX, DWORD PTR DS : [ESI + 0x58]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4
			LEA ECX, DWORD PTR DS : [ESI + 0x5C]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4
			ADD ESI, 0x60
			PUSH ESI
			MOV ECX, EDI
			CALL EDX
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4
			PUSH EBX
			MOV ECX, EDI
			CALL EDX
			POP EDI
			POP ESI
			POP EBX
			POP ECX
			RETN 0x4


		ACT_V3:
#ifdef DEBUG
			PUSHAD
			CALL Announce
			POPAD
#endif
			CALL ReadStringCall //szName
			LEA ECX, DWORD PTR DS : [ESI + 0x1C] //szLinkAniNameINT
			CALL ReadStringCall //szLinkAniName
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //dwLength
			LEA ECX, DWORD PTR DS : [ESI + 0x54]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX
			LEA ECX, DWORD PTR DS : [ESI + 0x38]
			CALL ReadStringCall //szNextActionName
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //dwBlendFrame
			LEA ECX, DWORD PTR DS : [ESI + 0x58]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX

			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //dwBlendFrame
			LEA ECX, DWORD PTR DS : [ESI + 0x5C]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX

			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //unk2
			LEA ECX, DWORD PTR DS : [ESI + 0x60]
			PUSH ECX
			MOV ECX, EDI
			CALL EDX

			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //unk1
			ADD ESI, 0x60
			PUSH ESI
			MOV ECX, EDI
			CALL EDX
			MOV EAX, DWORD PTR DS : [EDI]
			MOV EDX, DWORD PTR DS : [EAX + 0x20]
			PUSH 0x4 //nSignalCount
			PUSH EBX
			MOV ECX, EDI
			CALL EDX
			POP EDI
			POP ESI
			POP EBX
			POP ECX
			RETN 0x4

	END:
		POP EDI
		POP ESI
		POP EBX
		POP ECX
		RETN 0x4

	}



	VMProtectEnd();
}


void ActionManagerFix()
{
	SetRange((LPVOID)0x00C10F60, 0x1F, ASM::NOP);
	SetOp((LPVOID)0x00C10F60, (LPVOID)ActionManager, ASM::JMP);
}
