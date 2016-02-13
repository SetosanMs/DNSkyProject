#include <Windows.h>
#include "CharCreate.h"
#include "Memory.h"


const DWORD JMPBufferCharCreate = 0x00779B91;
const DWORD CALLBufferCharCreate = 0x00770150;
const DWORD CALLBufferCharCreate_After = 0x00724030;
DWORD *EBPSalvat = 0;
DWORD *SavePointers[10];

void *GetPointerToChars()
{
	return &SavePointers;
}

void LoadCharsIntoPointers()
{
	_asm{
		PUSHAD

		//Warrior
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA6478
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Archer
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA6488
		MOV DWORD PTR DS : [SavePointers], EAX
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Sorc
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA6498
		MOV DWORD PTR DS : [SavePointers + 0x04], EAX
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Cleric
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA64A8
		MOV DWORD PTR DS : [SavePointers + 0x08], EAX
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Academic
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA64B8
		MOV DWORD PTR DS : [SavePointers + 0x0C], EAX
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Kali
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA64C8
		MOV DWORD PTR DS : [SavePointers + 0x10], EAX
		//MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate
		//Assassin
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x00DA64D4
		MOV DWORD PTR DS : [SavePointers + 0x14], EAX
		//	MOV DWORD PTR DS : [ESI + 0x410], EAX
		CALL CALLBufferCharCreate

		//lencea/asta
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x01F22700
		MOV DWORD PTR DS : [SavePointers + 0x18], EAX
		//MOV DWORD PTR DS : [ESI + 0x414], EAX
		CALL CALLBufferCharCreate

		//machina
		PUSH EDI
		PUSH ESI
		MOV ECX, 0x01F22860
		MOV DWORD PTR DS : [SavePointers + 0x1C], EAX
		//MOV DWORD PTR DS : [ESI + 0x418], EAX //lencea
		CALL CALLBufferCharCreate

		MOV DWORD PTR DS : [SavePointers + 0x20], EAX

		POPAD
	}
}

//cea mai fututa chestie ever, nu a mai fost spatiu in pointer si au trebuit realocati.
const DWORD JMPCharCreatePointer1 = 0x00779CFC; //00779CFC   85C9             TEST ECX, ECX
Naked(CharCreatePointer1)
{
	//Init
	_asm{
		push edx
		LEA EDX, SavePointers
		MOV ECX, DWORD PTR DS : [EDX + EAX * 4]
		pop edx
		JMP JMPCharCreatePointer1
	}
}

const DWORD JMPCharCreatePointer2 = 0x00779D2F; //00779D2F | . 90                NOP
Naked(CharCreatePointer2)
{
	//back button!@
	_asm{
			
			LEA ESI, SavePointers
			MOV EDI, 0x9 //9 CLASE!
			JMP JMPCharCreatePointer2
	}
}

const DWORD JMPCharCreatePointer3 = 0x00779DD7; //00779DD7  |. D95C24 10         FSTP DWORD PTR SS:[ESP+0x10]
Naked(CharCreatePointer3)
{
	//Select Char
	_asm{
			LEA EBX, SavePointers
			JMP JMPCharCreatePointer3
	}
}

const DWORD JMPCharCreatePointer4 = 0x007393F6;//007393F6  |. 85C9           TEST ECX,ECX
Naked(CharCreatePointer4)
{
	//Select Char
	_asm{
		//MOV ECX, DWORD PTR DS : [EAX + ECX * 4 + 0x3FC];
		PUSH EDX
		LEA EDX, SavePointers
		MOV ECX, DWORD PTR DS : [EDX + ECX * 4]
		POP EDX
		JMP JMPCharCreatePointer4
	}
}

Naked(CharCreate)
{
//	DWORD *SavePointers[10];// = { 0 }; //create temporary pointers list!
	/*
	00779B79 | . 57             PUSH EDI; / Arg2
	00779B7A | . 56             PUSH ESI; | Arg1
	00779B7B | .B9 D464DA00    MOV ECX, dragonne.00DA64D4; | ASCII "ID_RBT_ASSASSIN"
	00779B80 | . 8986 10040000  MOV DWORD PTR DS : [ESI + 410], EAX; |
	00779B86 | .E8 C565FFFF    CALL dragonne.00770150; \dragonne.00770150
	*/
	//	DWORD CallBuff;
	//	DWORD JMPBuff;
	_asm{
		MOV DWORD PTR DS : [ESI + 0x414], EAX
	}

	LoadCharsIntoPointers();

	_asm{
			
			//MOV DWORD PTR DS : [ESI + 0x418], EAX
			//MOV DWORD PTR DS : [ESI + 0x41C], EAX //machina

			ADD ESI, 0x418
			MOV EBPSalvat, EBP
			MOV EBP, CALLBufferCharCreate_After
			LEA EDX, DWORD PTR SS : [ESP + 0x10]
			PUSH EDX
			LEA EAX, DWORD PTR SS : [ESP + 0x1C]
			PUSH EAX
			PUSH ESI
			MOV DWORD PTR SS : [ESP + 0x10], 0x7
			MOV DWORD PTR SS : [ESP + 0x20], 0x7
			CALL CALLBufferCharCreate_After
			LEA EDX, DWORD PTR SS : [ESP + 0x10]
			PUSH EDX
			LEA EAX, DWORD PTR SS : [ESP + 0x1C]
			PUSH EAX
			PUSH ESI
			MOV DWORD PTR SS : [ESP + 0x10], 0x8
			MOV DWORD PTR SS : [ESP + 0x20], 0x8
			CALL CALLBufferCharCreate_After
			SUB ESI, 0x418
			MOV EBP, DWORD PTR DS : EBPSalvat
			


			//go back to original position
			//00779B8B   8986 14040000    MOV DWORD PTR DS : [ESI + 414], EAX
			JMP JMPBufferCharCreate
	}
}


const DWORD JMPBufferCharCreate2 = 0x00779C63;
const DWORD CALLBufferCharCreate2 = 0x00724030;
Naked(CharCreate2)
{
	//	DWORD LoadCharDataCall;
	//	DWORD ReturnJump;
	/*
	00779C57  |. 52             PUSH EDX                                 ; /Arg3
	00779C58  |. 8D4424 1C      LEA EAX,DWORD PTR SS:[ESP+1C]            ; |
	00779C5C  |. 50             PUSH EAX                                 ; |Arg2
	00779C5D  |. 56             PUSH ESI                                 ; |Arg1
	00779C5E  |. E8 CDA3FAFF    CALL dragonne.00724030                   ; \dragonne.00724030
	*/
	_asm{
			//Assasin
			PUSH EDX
			LEA EAX, DWORD PTR SS : [ESP + 0x1C]
			PUSH EAX
			PUSH ESI
			//MOV LoadCharDataCall, 0x00724030
			CALL CALLBufferCharCreate2
			//required free space
			LEA ECX, DWORD PTR SS : [ESP + 0x10] //00779C4B  |. 8D5424 10      LEA EDX,DWORD PTR SS:[ESP+10]
			//Lencea from CHN
			/*
			007E36D9 | . 51             PUSH ECX; / Arg3
			007E36DA | . 8D5424 1C      LEA EDX, DWORD PTR SS : [ESP + 1C]; |
			007E36DE | .B8 07000000    MOV EAX, 7; |
			007E36E3 | . 52             PUSH EDX; | Arg2
			007E36E4 | . 56             PUSH ESI; | Arg1
			007E36E5 | . 894424 1C      MOV DWORD PTR SS : [ESP + 1C], EAX; |
			007E36E9 | . 894424 20      MOV DWORD PTR SS : [ESP + 20], EAX; |
			007E36ED | .E8 DEA40F00    CALL DragonNe.008DDBD0; \DragonNe.008DDBD0
			*/
			PUSH EDX
			LEA EAX, DWORD PTR SS : [ESP + 0x1C]
			PUSH EAX
			PUSH ESI
			CALL CALLBufferCharCreate2
			LEA ECX, DWORD PTR SS : [ESP + 0x10] //00779C4B  |. 8D5424 10      LEA EDX,DWORD PTR SS:[ESP+10]
			/*
			PUSH ECX
			LEA EDX, DWORD PTR SS : [ESP + 0x1C]
			MOV EAX, 7
			PUSH EDX
			PUSH ESI
			MOV DWORD PTR SS : [ESP + 0x1C], EAX
			MOV DWORD PTR SS : [ESP + 0x20], EAX*/
			//	MOV LoadCharDataCall, 0x00724030
			CALL CALLBufferCharCreate2



			//	MOV ECX, DWORD PTR SS : [ESP + 44] //overflow?

			//00779C63 | . 8B4C24 44      MOV ECX, DWORD PTR SS : [ESP + 44]
			//MOV ReturnJump, 0x00779C63
			JMP JMPBufferCharCreate2
	}
}

const DWORD JMPBufferCharCreate3 = 0x007785A7;
const DWORD CALLBufferCharCreate3 = 0x0042D7A0;
Naked(CharCreate3)
{
	//	DWORD CallPointer;
	//	DWORD JMPPTR;
	/*
	ASSASIN =
	00778596 | . 53             PUSH EBX; / Arg2
	00778597 | . 68 3063DA00    PUSH dragonne.00DA6330; | Arg1 = 00DA6330 ASCII "ID_STATIC_ASSASSIN"
	0077859C | . 8BCD           MOV ECX, EBP; |
	0077859E | . 894424 34      MOV DWORD PTR SS : [ESP + 34], EAX; |
	007785A2 | .E8 F951CBFF    CALL dragonne.0042D7A0; \dragonne.0042D7A0
	LENCEA =
	007E1F98  |. 53             PUSH EBX                                 ; /Arg2
	007E1F99  |. 68 BC2AEA00    PUSH DragonNe.00EA2ABC                   ; |Arg1 = 00EA2ABC ASCII "ID_STATIC_LENCEA"
	007E1F9E  |. 8BCF           MOV ECX,EDI                              ; |
	007E1FA0  |. 894424 38      MOV DWORD PTR SS:[ESP+38],EAX            ; |
	007E1FA4  |. E8 876BC5FF    CALL DragonNe.00438B30                   ; \DragonNe.00438B30


	*/
	_asm{
		//Assasin
		PUSH EBX
			PUSH 0x00DA6330
			MOV ECX, EBP
			MOV DWORD PTR SS : [ESP + 0x34], EAX
			CALL CALLBufferCharCreate3
			//Lencea
			PUSH EBX
			PUSH 0x01F22720
			MOV ECX, EBP
			MOV DWORD PTR SS : [ESP + 0x38], EAX
			CALL CALLBufferCharCreate3
			//Avenger
			PUSH EBX
			PUSH 0x01F22880
			MOV ECX, EBP
			MOV DWORD PTR SS : [ESP + 0x3C], EAX
			CALL CALLBufferCharCreate3
			//Machina
			PUSH EBX
			PUSH 0x01F227D0
			MOV ECX, EBP
			MOV DWORD PTR SS : [ESP + 0x40], EAX
			CALL CALLBufferCharCreate3

			JMP JMPBufferCharCreate3
	}
	//buffer overflow?
	//00778521.C74424 50 0700>MOV DWORD PTR SS : [ESP + 50], 7


}

const DWORD CALLBufferCreateChar4 = 0x0042D7A0;
const DWORD JMPBufferCreateChar4 = 0x00908564;
Naked(CharCreate4) //lencea icon
{
	/*
	ASS
	00908545  |. 6A 00          PUSH 0                                   ; /Arg2 = 00000000
	00908547  |. 68 ACA8DB00    PUSH dragonne.00DBA8AC                   ; |Arg1 = 00DBA8AC ASCII "ID_ICON_ASSASSIN"
	0090854C  |. 8BCB           MOV ECX,EBX                              ; |
	0090854E  |. 8983 F4030000  MOV DWORD PTR DS:[EBX+3F4],EAX           ; |
	00908554  |. E8 4752B2FF    CALL dragonne.0042D7A0                   ; \dragonne.0042D7A0

	CHINA
	LENCEA =
	00998D85  |. 6A 00          PUSH 0                                   ; /Arg2 = 00000000
	00998D87  |. 68 6C8BEC00    PUSH dragonne.00EC8B6C                   ; |Arg1 = 00EC8B6C ASCII "ID_ICON_LENCEA"
	00998D8C  |. 8BCF           MOV ECX,EDI                              ; |
	00998D8E  |. 8987 08040000  MOV DWORD PTR DS:[EDI+408],EAX           ; |
	00998D94  |. E8 F7FAA9FF    CALL dragonne.00438890                   ; \dragonne.00438890
	*/
	_asm{
		//Assasin
		PUSH 0
			PUSH 0x00DBA8AC
			MOV ECX, EBX
			MOV DWORD PTR DS : [EBX + 0x3F4], EAX
			CALL CALLBufferCreateChar4
			//Lencea
			PUSH 0
			PUSH 0x01F22780
			MOV ECX, EBX
			MOV DWORD PTR SS : [EBX + 0x3F8], EAX
			CALL CALLBufferCreateChar4
			//machina
			PUSH 0
			PUSH 0x01F228A0
			MOV ECX, EBX
			MOV DWORD PTR SS : [EBX + 0x3FC], EAX
			CALL CALLBufferCreateChar4

			MOV DWORD PTR DS : [EBX + 0x400], EAX //increase buffer
			MOV EDI, 9 //8 class check!, make it 9

			//MOV JMPPTR, 0x00908564 //00908564  |> 833E 00        /CMP DWORD PTR DS:[ESI],0
			JMP JMPBufferCreateChar4
	}
}



void CharCreateFix()
{
#ifdef DEBUG
#endif
// new	00779B13 | . 57             PUSH EDI; / Arg2 = > 00000000
	//SetRange((LPVOID)0x00779B13, 0x80-2, ASM::NOP); //18+ 6
	//SetOp((LPVOID)0x00779B13, (LPVOID)CharCreate, ASM::JMP);

	SetRange((LPVOID)0x00779B8B, 6, ASM::NOP); //	00779B8B | . 8986 14040000  MOV DWORD PTR DS : [ESI + 0x414], EAX
	SetOp((LPVOID)0x00779B8B, (LPVOID)CharCreate, ASM::JMP);
	/**/
#ifdef _9JOB
	SetByte((PVOID)(0x00779CF0 + 2), 0x08);//init ceva	00779CF0     83F8 07        CMP EAX, 0x7;  //aici 8
#else	
	SetByte((PVOID)(0x00779CF0 + 2), 0x07);//init ceva	00779CF0     83F8 07        CMP EAX, 0x7;  //aici 8
#endif
	SetByte((PVOID)(0x00779B0C + 2), 0x09);// static text 00779B0C   83FF 09        CMP EDI, 0x9;  AICI VINE 9
	SetByte((PVOID)(0x00779E12 + 2), 0x09);// select character 00779E12     83FE 08        CMP ESI, 0x8
	
	//?
	SetByte((PVOID)(0x00779E32 + 1), 0x07);//aici vine crash tot la select char 00779E32     BB 08000000    MOV EBX,0x8
	SetByte((PVOID)(0x00779E9D + 2), 0x06);//tot la select char	00779E9D | . 83F8 06        CMP EAX, 0x6
	
	
//	SetByte((PVOID)(0x00779B9B + 2), 0x14);//00779B9B     81C6 20040000  ADD ESI, 0x420

	SetByte((PVOID)(0x00779C46 + 1), 0x06);// RBT load ? 00779C46 | .B8 06000000    MOV EAX, 0x6


	SetByte((PVOID)(0x007393EA + 2), 0x08);// TOATA MAGIA AICI idk 007393EA | . 83F9 06        CMP ECX, 0x6
	//SetByte((PVOID)(0x00779D2A + 1), 0x08);//Reset Back Button == 00779D2A     BF 09000000    MOV EDI,0x9 

	
	//00779CF5  |. 8B8C81 FC030000   MOV ECX,DWORD PTR DS:[ECX+EAX*4+0x3FC]
	SetRange((LPVOID)0x00779CF5, 7, ASM::NOP); 
	SetOp((LPVOID)0x00779CF5, (LPVOID)CharCreatePointer1, ASM::JMP);//La initializare


	//00779D24  |. 81C6 FC030000     ADD ESI,0x3FC         ;  back button
	SetRange((LPVOID)0x00779D24, 12, ASM::NOP);
	SetOp((LPVOID)0x00779D24, (LPVOID)CharCreatePointer2, ASM::JMP);
	

	//00779DD1 | . 8D9F FC030000     LEA EBX, DWORD PTR DS : [EDI + 0x3FC];  select char
	SetRange((LPVOID)0x00779DD1, 6, ASM::NOP);
	SetOp((LPVOID)0x00779DD1, (LPVOID)CharCreatePointer3, ASM::JMP);


	//007393EF | . 8B8C88 FC03000>MOV ECX, DWORD PTR DS : [EAX + ECX * 4 + 0x3FC]; magie cand dai click!
	SetRange((LPVOID)0x007393EF, 6, ASM::NOP);
	SetOp((LPVOID)0x007393EF, (LPVOID)CharCreatePointer4, ASM::JMP);
	


	//SetByte((PVOID)0x00779B0E, 0x08); //00779B0C   83FF 07 | CMP EDI, 7

//	SetRange((LPVOID)0x00779C57, 12, ASM::NOP);
//	SetOp((LPVOID)0x00779C57, (LPVOID)CharCreate2, ASM::JMP);



	//00779C57  |. 52             PUSH EDX                                 ; /Arg3
	SetRange((LPVOID)0x00778596, 17, ASM::NOP);
	SetOp((LPVOID)0x00778596, (LPVOID)CharCreate3, ASM::JMP);
	SetByte((PVOID)0x007785C1, 0x08);//007785C1   83FE 07 | CMP ESI, 7
	//INCREASE THE BUFFER TO AVOID OVERFLOW.
	SetByte((PVOID)0x007785AA, 0x3C); //007785A7   894424 30        MOV DWORD PTR SS : [ESP + 30], EAX ++ added machina
	SetByte((PVOID)(0x007785BF + 2), 0x0A); // 09 nu 0A! e schimbat darkavenger cu machina --text sau icon 007785BF     83FE 0A        CMP ESI, 0xA


	//lencea button 0x1F 00908545  |. 6A 00          PUSH 0                                   ; /Arg2 = 00000000
	SetRange((LPVOID)0x00908545, 0x1F, ASM::NOP);
	SetOp((LPVOID)0x00908545, (LPVOID)CharCreate4, ASM::JMP);



	//OK!
	//new
	//SetByte((PVOID)(0x00779CF0 + 2), 0x08); //	00779CF0     83F8 08        CMP EAX, 0x8
	//SetByte((PVOID)(0x00779D2A + 1), 0x09); //	00779D2A | .BF 07000000    MOV EDI, 0x7

/*	SetByte((PVOID)(0x00779CF5 + 3), 0x14); //	00779CF5     8B8C81 1404000>MOV ECX,DWORD PTR DS:[ECX+EAX*4+0x414]
	SetByte((PVOID)(0x00779CF5 + 4), 0x04); //	00779CF5     8B8C81 1404000>MOV ECX,DWORD PTR DS:[ECX+EAX*4+0x414]

	SetByte((PVOID)(0x00779D24 + 2), 0x14); //	00779D24     8B8C81 1404000>MOV ECX,DWORD PTR DS:[ECX+EAX*4+0x414]
	SetByte((PVOID)(0x00779D24 + 3), 0x04); //	00779D24     8B8C81 1404000>MOV ECX,DWORD PTR DS:[ECX+EAX*4+0x414]


	SetByte((PVOID)(0x0047E47E + 3), 0x14); //   //	0047E47E     8B8488 1404000>MOV EAX, DWORD PTR DS : [EAX + ECX * 4 + 0x414]
	SetByte((PVOID)(0x0047E47E + 4), 0x04); ////	0047E47E     8B8488 1404000>MOV EAX, DWORD PTR DS : [EAX + ECX * 4 + 0x414]
	

	SetByte((PVOID)(0x0047E47E + 3), 0x14); //007393EF     8B8C88 1404000>MOV ECX, DWORD PTR DS : [EAX + ECX * 4 + 0x414]
	SetByte((PVOID)(0x0047E47E + 4), 0x04); //007393EF     8B8C88 1404000>MOV ECX, DWORD PTR DS : [EAX + ECX * 4 + 0x414]


	SetByte((PVOID)(0x00779DD1 + 2), 0x14); //	00779DD1     8D9F 14040000  LEA EBX, DWORD PTR DS : [EDI + 0x414]
	SetByte((PVOID)(0x00779DD1 + 3), 0x04); //	00779DD1     8D9F 14040000  LEA EBX, DWORD PTR DS : [EDI + 0x414]
	*/

}