#include <Windows.h>
#include "Memory.h"

/*
00A9E6D0   B8 04C80000      MOV EAX, 0xC804
00A9E6D5   E8 C6471C00      CALL dragonne.00C62EA0
00A9E6DA   57               PUSH EDI
00A9E6DB   68 03C80000      PUSH 0xC803
00A9E6E0   8D4424 08        LEA EAX, DWORD PTR SS : [ESP + 0x8]
00A9E6E4   6A 00            PUSH 0x0
00A9E6E6   50               PUSH EAX
00A9E6E7   E8 B46F1B00      CALL dragonne.00C556A0
00A9E6EC   8A9424 1CC80000  MOV DL, BYTE PTR SS : [ESP + 0xC81C]
00A9E6F3   8A8424 20C80000  MOV AL, BYTE PTR SS : [ESP + 0xC820]
00A9E6FA   8D4E 04          LEA ECX, DWORD PTR DS : [ESI + 0x4]
00A9E6FD   66 : 894C24 13     MOV WORD PTR SS : [ESP + 0x13], CX
00A9E702   8B8C24 24C80000  MOV ECX, DWORD PTR SS : [ESP + 0xC824]
00A9E709   56               PUSH ESI
00A9E70A   885424 19        MOV BYTE PTR SS : [ESP + 0x19], DL
00A9E70E   51               PUSH ECX
00A9E70F   8D5424 1F        LEA EDX, DWORD PTR SS : [ESP + 0x1F]
00A9E713   52               PUSH EDX
00A9E714   884424 22        MOV BYTE PTR SS : [ESP + 0x22], AL
00A9E718   E8 D30C1B00      CALL dragonne.00C4F3F0
00A9E71D   8B4424 1F        MOV EAX, DWORD PTR SS : [ESP + 0x1F]
00A9E721   8B15 201E6D01    MOV EDX, DWORD PTR DS : [0x16D1E20]
00A9E727   83C0 03          ADD EAX, 0x3
00A9E72A   83C4 18          ADD ESP, 0x18
00A9E72D   8D4C24 07        LEA ECX, DWORD PTR SS : [ESP + 0x7]
00A9E731   66 : 894424 04     MOV WORD PTR SS : [ESP + 0x4], AX
00A9E736   0FB74424 07      MOVZX EAX, WORD PTR SS : [ESP + 0x7]
00A9E73B   51               PUSH ECX
00A9E73C   52               PUSH EDX
00A9E73D   E8 CEF9FFFF      CALL dragonne.00A9E110
00A9E742   8B8C24 0CC80000  MOV ECX, DWORD PTR SS : [ESP + 0xC80C]
00A9E749   0FB77C24 04      MOVZX EDI, WORD PTR SS : [ESP + 0x4]
00A9E74E   8D4424 04        LEA EAX, DWORD PTR SS : [ESP + 0x4]
00A9E752   50               PUSH EAX
00A9E753   8B41 10          MOV EAX, DWORD PTR DS : [ECX + 0x10]
00A9E756   E8 95CBFFFF      CALL dragonne.00A9B2F0
00A9E75B   33D2             XOR EDX, EDX
00A9E75D   85C0             TEST EAX, EAX
00A9E75F   0F9DC2           SETGE DL
00A9E762   5F               POP EDI
00A9E763   83EA 01          SUB EDX, 0x1
00A9E766   8BC2             MOV EAX, EDX
00A9E768   81C4 04C80000    ADD ESP, 0xC804
00A9E76E   C2 1000          RETN 0x10
*/

const DWORD Call1 = 0x00C62EA0;
const DWORD Call2 = 0x00C556A0;
const DWORD Call3 = 0x00C4F3F0;
const DWORD EncryptPacket = 0x00A9E110;
const DWORD Call5 = 0x00A9B2F0;
Naked(SendVirtualize)
{
	_asm{
		MOV EAX, 0xC804
		CALL Call1
		PUSH EDI
		PUSH 0xC803
		LEA EAX, DWORD PTR SS : [ESP + 0x8]
		PUSH 0x0
		PUSH EAX
		CALL Call2
		MOV DL, BYTE PTR SS : [ESP + 0xC81C]
		MOV AL, BYTE PTR SS : [ESP + 0xC820]
		LEA ECX, DWORD PTR DS : [ESI + 0x4]
		MOV WORD PTR SS : [ESP + 0x13], CX
		MOV ECX, DWORD PTR SS : [ESP + 0xC824]
		PUSH ESI
		MOV BYTE PTR SS : [ESP + 0x19], DL
		PUSH ECX
		LEA EDX, DWORD PTR SS : [ESP + 0x1F]
		PUSH EDX
		MOV BYTE PTR SS : [ESP + 0x22], AL
		CALL Call3
		MOV EAX, DWORD PTR SS : [ESP + 0x1F]
		MOV EDX, DWORD PTR DS : [0x16D1E20]
		ADD EAX, 0x3
		ADD ESP, 0x18
		LEA ECX, DWORD PTR SS : [ESP + 0x7]
		MOV WORD PTR SS : [ESP + 0x4], AX
		MOVZX EAX, WORD PTR SS : [ESP + 0x7]
		PUSH ECX
		PUSH EDX
		CALL EncryptPacket //encrypt packet
		MOV ECX, DWORD PTR SS : [ESP + 0xC80C]
		MOVZX EDI, WORD PTR SS : [ESP + 0x4]
		LEA EAX, DWORD PTR SS : [ESP + 0x4]
		PUSH EAX
		MOV EAX, DWORD PTR DS : [ECX + 0x10]
		CALL Call5
		XOR EDX, EDX
		TEST EAX, EAX
		SETGE DL
		POP EDI
		SUB EDX, 0x1
		MOV EAX, EDX
		ADD ESP, 0xC804
		RETN 0x10
	}
}

void HookSendFunction()
{
	SetRange((LPVOID)0x00A9E6D0, 0xA1, ASM::NOP); // 00BDF470 / $ 81EC 14010000  SUB ESP, 0x114
	SetOp((LPVOID)0x00A9E6D0, (LPVOID)SendVirtualize, ASM::JMP);
}