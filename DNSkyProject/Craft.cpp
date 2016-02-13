#include <Windows.h>
#include "Craft.h"
#include "Memory.h"

CONST DWORD CCF_CALL1 = 0x00AAF180;
CONST DWORD CCF_CALL2 = 0x00AE6B90;
void __declspec(naked) CraftCompoundFix()
{
	DWORD jumpaddr;
	_asm{
		//asssasin
		PUSH EAX
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x7
		PUSH 0x133
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3E0]
		PUSH EAX
		CALL CCF_CALL2
		//lencea
		PUSH EAX
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x8
		PUSH 0x134
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3E0]
		PUSH EAX
		CALL CCF_CALL2
		//machina
		PUSH EAX
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x8
		PUSH 0x178
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3E0]
		PUSH EAX
		CALL CCF_CALL2

		MOV jumpaddr, 0x00A034A9  // | .A1 40172201    MOV EAX, DWORD PTR DS : [1221740]
		JMP jumpaddr
	}
}

void __declspec(naked) CraftCompoundFixAccessories()
{
	DWORD jumpaddr;

	_asm{
		//assasin
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x7
		PUSH 0x0
		PUSH 0x133
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3EC]
		PUSH EAX
		CALL CCF_CALL2
		//lencea
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x8
		PUSH 0x0
		PUSH 0x134
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3EC]
		PUSH EAX
		CALL CCF_CALL2
		//machina
		MOV EAX, DWORD PTR DS : [0x16D1DE8]
		PUSH 0x1
		PUSH 0x8
		PUSH 0x0
		PUSH 0x178
		PUSH 0x1
		CALL CCF_CALL1
		MOV ECX, DWORD PTR DS : [EDI + 0x3EC]
		PUSH EAX
		CALL CCF_CALL2

		mov jumpaddr, 0x009FAA19
		JMP jumpaddr
	}
}


void CraftFix()
{
	//Fix Lencea Craft 00A03486  |. A1 E81D6D01    MOV EAX,DWORD PTR DS:[16D1DE8]
	SetRange((LPVOID)0x00A03486, 0x1F + 4, ASM::NOP);
	SetOp((LPVOID)0x00A03490, (LPVOID)CraftCompoundFix, ASM::JMP); //00A03490     90             NOP
	SetByte((PVOID)(0x00A0387E + 2), 0x09);

	//Fix Lencea Craft Accessories
	SetRange((LPVOID)0x009FA9F6, 0x1E + 4, ASM::NOP); //009FA9F6     A1 E81D6D01    MOV EAX, DWORD PTR DS : [16D1DE8]
	SetOp((LPVOID)0x009FA9F6, (LPVOID)CraftCompoundFixAccessories, ASM::JMP); //009FA9F6     A1 E81D6D01    MOV EAX, DWORD PTR DS : [16D1DE8]
	SetByte((PVOID)(0x009FAD39 + 2), 0x09);

}