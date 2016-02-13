#include <Windows.h>
#include "Memory.h"
#include "ClassInit.h"

Naked(FieldClassInit)
{
	DWORD ClassID;
	DWORD ActPointer;
	DWORD LoadAct_Buff;

	_asm
	{
		mov ClassID, eax
		add ClassID, 0xB
	}

	switch (ClassID)
	{
	case 0xB: //ASCII "warrior_skill_1st_swordmaster.act"; Case B of switch 005E1D77
		ActPointer = 0x0D815B4;
		break;
	case 0xC://ASCII "warrior_skill_1st_mercenary.act"; Case C of switch 005E1D77
		ActPointer = 0x0D815D8;
		break;
	case 0x17:
		ActPointer = 0x0D815F8;
		break;
	case 0x18:
		ActPointer = 0x0D81618;
		break;
	case 0x19:
		ActPointer = 0x0D81638;
		break;
	case 0x1A:
		ActPointer = 0x0D81658;
		break;
	case 0xE:
		ActPointer = 0x0D81678;
		break;
	case 0xF:
		ActPointer = 0x0D81698;
		break;
	case 0x1D:
		ActPointer = 0x0D816B8;
		break;
	case 0x1E:
		ActPointer = 0x0D816D4;
		break;
	case 0x1F:
		ActPointer = 0x0D816F4;
		break;
	case 0x20:
		ActPointer = 0x0D81714;
		break;
	case 0x11:
		ActPointer = 0x0D81734;
		break;
	case 0x12:
		ActPointer = 0x0D8175C;
		break;
	case 0x23:
		ActPointer = 0x0D81780;
		break;
	case 0x24:
		ActPointer = 0x0D817A0;
		break;
	case 0x25:
		ActPointer = 0x0D817C0;
		break;
	case 0x26:
		ActPointer = 0x0D817E0;
		break;
	case 0x14:
		ActPointer = 0x0D81800;
		break;
	case 0x16:
		ActPointer = 0x0D81820;
		break;
	case 0x29:
		ActPointer = 0x0D8183C;
		break;
	case 0x2A:
		ActPointer = 0x0D8185C;
		break;
	case 0x2B:
		ActPointer = 0x0D8187C;
		break;
	case 0x2C:
		ActPointer = 0x0D81898;
		break;
	case 0x2E:
		ActPointer = 0x0D818B8;
		break;
	case 0x30:
		ActPointer = 0x0D818FC;
		break;
	case 0x31:
		ActPointer = 0x0D81920;
		break;
	case 0x32:
		ActPointer = 0x0D81944;
		break;
	case 0x33:
		ActPointer = 0x0D81964;
		break;
	case 0x36:
		ActPointer = 0x0D81988;
		break;
	case 0x37:
		ActPointer = 0x0D819A4;
		break;
	case 0x38:
		ActPointer = 0x0D819C4;
		break;
	case 0x39:
		ActPointer = 0x0D819E4;
		break;
	case 0x3A:
		ActPointer = 0x0D81A00;
		break;
	case 0x3B:
		ActPointer = 0x0D81A20;
		break;
	case 0x3E:
		ActPointer = 0x0D81A40;
		break;
	case 0x3F:
		ActPointer = 0x0D81A60;
		break;
	case 0x40:
		ActPointer = 0x0D81A80;
		break;
	case 0x43:
		ActPointer = 0x0D81AA0;
		break;
	case 0x44:
		ActPointer = 0x0D81AC0;
		break;
	case 0x45:
		ActPointer = 0x0D81AE4;
		break;
	case 0x48: //lencea "lencea_skill_1st_piercer.act"; Case 48 of switch 00612C27
		ActPointer = 0x01F21651;
		break;
	case 0x49:
		ActPointer = 0x01F21671;
		break;
	case 0x4A:
		ActPointer = 0x01F21691;
		break;
	case 0x4B: //Dark Avenger
		ActPointer = 0x01F21820;
		break;
	case 0x4C:
		ActPointer = 0x01F21840;
		break;
	case 0x4D: //Machina
		ActPointer = 0x01F21710;
		break;
	case 0x4E:
		ActPointer = 0x01F21730;
		break;
	case 0x4F:
		ActPointer = 0x01F21750;
		break;
		//Silver Hunter
	default:
#ifdef DEBUG_CLASS
		char buf[127];
		wsprintf(buf, enc(" GOT INVALID CLASS ID = %X"), ClassID);
		MessageBoxA(NULL, buf, enc("NON-OK"), MB_OK);
#endif
		break;
	}

#ifdef DEBUG_CLASS
	char buf[127];
	wsprintf(buf, enc(" GOT CLASS ID = %X"), ClassID);
	MessageBoxA(NULL, buf, enc("OK"), MB_OK);
#endif


	_asm
	{
		push ActPointer
			mov LoadAct_Buff, 0x005E1EF3
			jmp LoadAct_Buff
	}

}

Naked(FieldBaseClassInit)
{
	DWORD BaseClassID;
	DWORD BaseClassPointer;
	DWORD JumpPointer;

	_asm{
		mov BaseClassID, eax
			add BaseClassID, 1
	}

	switch (BaseClassID)
	{
	case 1:
		BaseClassPointer = 0x0D814D8;
		break;
	case 2:
		BaseClassPointer = 0x0D814E0;
		break;
	case 3:
		BaseClassPointer = 0x0D814E8;
		break;
	case 4:
		BaseClassPointer = 0x0D814F4;
		break;
	case 5:
		BaseClassPointer = 0x0D814FC;
		break;
	case 6:
		BaseClassPointer = 0x0D81508;
		break;
	case 7:
		BaseClassPointer = 0x0D81510;
		break;
	case 8:
		BaseClassPointer = 0x01F22760;
		break;
	case 9:
		BaseClassPointer = 0x01F227C0;
		break;
	default:
#ifdef DEBUG_CLASS
		char buf[127];
		wsprintf(buf, enc(" UNKOWN CLASS ID = %X"), BaseClassID);
		MessageBoxA(NULL, buf, enc("NON-OK"), MB_OK);
#endif
		break;
	}

#ifdef DEBUG_CLASS
	char buf[127];
	wsprintf(buf, enc(" GOT CLASS ID = %X"), BaseClassID);
	//MessageBoxA(NULL, buf, "OK", MB_OK);
#endif

	_asm
	{
		push BaseClassPointer
			mov JumpPointer, 0x005E166E
			jmp JumpPointer
	}

}



void ClassInitFix()
{
	//set Jump to naked function. jumptable fieldclass
	SetRange((LPVOID)0x005E1D83, 17, ASM::NOP); //7+5+5 //005E1D83  |. FF2485 EC215E00     JMP DWORD PTR DS:[EAX*4+5E21EC]
	SetOp((LPVOID)0x005E1D83, (LPVOID)FieldClassInit, ASM::JMP);
	SetByte((PVOID)0x005E1D7C, 0x44); //0x3F+0xB = total classes , 0x41+0x0B = including dark avenger 


	//Init base classes 
	SetRange((LPVOID)0x005E1638, 54, ASM::NOP); // 005E1638   FF2485 D0215E00  JMP DWORD PTR DS:[EAX*4+5E21D0] JUMPTABLE
	SetOp((LPVOID)0x005E1638, (LPVOID)FieldBaseClassInit, ASM::JMP);
	SetByte((PVOID)0x005E1635, 0x08); //005E1633   83F8 06          CMP EAX,6 + added machina
}