#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "se.h"
#include "PackingSystem.h"
#include "zLibUncompress.h"
#include "Memory.h"


//realoc toata functia in memoria dll-ului ca sa nu faca competitia hook si sa-mi fure fisierele decriptate. burn em' all.
//REGISTRI ebx = filename , esi = pointer catre uncompresed data.
//CEtFileHandle *CEtPackingFile::OpenFile( const char *strPath )
const DWORD Call1 = 0x00BE6790;
const DWORD Call2 = 0x00BE5C60;
const DWORD Call3 = 0x00BE5FB0;
const DWORD Call4 = 0x00C40FEF;
const DWORD Call5 = 0x00BE6000;
const DWORD Call6 = 0x00B51BF0;
const DWORD Call7 = 0x00C44BED;
const DWORD Call8 = 0x00C450E0;
const DWORD Call9 = 0x00BE5CA0;
DWORD PointerFisier = 0;
DWORD PointerData = 0;
DWORD PointerDataSize = 0;
BOOL isCrypted = FALSE;
FileVersion result = ANOTHERFILE;
DWORD EAXSalvat = 0;
DWORD EDXSalvat = 0;
DWORD ECXSalvat = 0;
DWORD AdresaPointer = 0;
DWORD AdresaSizePointer = 0;
Naked(etOpenFile)
{
	_asm{
			SUB ESP, 0xC //;  zlib uncompress
			PUSH EBX
			PUSH EBP
			PUSH ESI
			MOV ESI, EAX
			CMP DWORD PTR DS : [ESI + 0x8], -0x1
			PUSH EDI
			JE RETURN1
			PUSH ESI //; / Arg1
			CALL Call1 //; \dragonne.00BE6790
			CMP EAX, -0x1
			JE RETURN1
			MOV EDI, EAX
			CALL Call2
			MOV EBX, EAX
			TEST EBX, EBX
			JE RETURN1
			MOV EAX, DWORD PTR DS : [ESI + 0x8]
			CMP EAX, -0x1
			JE JUMP1
			MOV ECX, DWORD PTR DS : [EBX + 0x10C]
			PUSH 0x0 //; / Arg3 = 00000000
			PUSH ECX //; | Arg2
			PUSH EAX //; | Arg1
			XOR EDX, EDX //; |
			CALL Call3 //; \dragonne.00BE5FB0
			JMP JUMP1
JUMP1 :
			MOV EDX, DWORD PTR DS : [EBX + 0x100]
			PUSH EDX
			CALL Call4
			MOV EDX, DWORD PTR DS : [ESI + 0x8]
			ADD ESP, 0x4
			CMP EDX, -0x1
			MOV EBP, EAX
			JE JUMP2
			MOV ESI, DWORD PTR DS : [EBX + 0x100]
			MOV ECX, EBP
			CALL Call5
			JMP JUMP2
JUMP2 :
			MOV EDI, DWORD PTR DS : [EBX + 0x104]
			PUSH EDI
			MOV DWORD PTR SS : [ESP + 0x14], EDI
			CALL Call4
			MOV ECX, DWORD PTR DS : [EBX + 0x100]
			MOV ESI, EAX
			ADD ESP, 0x4
			MOV DWORD PTR SS : [ESP + 0x14], EDI

			MOV PointerDataSize, EDI
			
			PUSH ESI
			MOV EAX, EBP
			LEA EDI, DWORD PTR SS : [ESP + 0x14]
			MOV DWORD PTR SS : [ESP + 0x1C], ESI
			CALL Call6 //decompress
			
			/*aici undeva ma infig*/
			PUSHAD

			MOV PointerData, ESI
			MOV PointerFisier, EBX
	}

	FileCrypt.CheckFileType((char*)PointerFisier, (BYTE*)PointerData, PointerDataSize);
	//printf("[ZLIB]LoadFile(SIZE %d): %s \n", PointerDataSize, (CHAR*)PointerFisier);
	/* DEPRECATED.
	result = FileCrypt.CheckFileType((char*)PointerFisier, (BYTE*)PointerData, PointerDataSize);
	if (result > 0 && result < 10)
	{
		isCrypted = TRUE;
		decrypt_LZO((BYTE*)PointerData, PointerDataSize, result);
		//AdresaPointer = (DWORD)&dBuffer[fCount];
		//AdresaSizePointer = (DWORD)&fSizeBuffer[fCount];
		//fCount++;
	}
	else{
		//isCrypted = FALSE;
	}*/


	_asm{
		POPAD
/*
		CMP isCrypted, 1
		JE WriteNewFile
		JMP ContinueWriteFile

WriteNewFile:
		//set new pointer to esi
		//set new size
		MOV EAXSalvat, EAX
		//MOV EDXSalvat, EDX
		MOV ECXSalvat, ECX


		MOV EAX, AdresaSizePointer
		MOV ECX, DWORD PTR DS : [EAX]
		//MOV DWORD PTR SS : [ESP + 0x10], ECX //orig size
		MOV DWORD PTR SS : [ESP + 0x14], ECX //decompress size
		MOV DWORD PTR SS : [ESP + 0x18], ECX //decompress size
		MOV DWORD PTR SS : [EBX + 0x104], ECX //decompress size

		MOV EAX, AdresaPointer
		MOV ECX, DWORD PTR DS : [EAX]
		MOV DWORD PTR SS : [ESP + 0x1C], ECX //FilePointer
		//MOV DWORD PTR SS : [ESP], ECX //FilePointer
		//MOV ESI, ECX

		MOV EAX, EAXSalvat
		//MOV EDX, EDXSalvat
		MOV ECX, ECXSalvat
		//ADD fCount, 1 // fcount++ :)
		JMP ContinueWriteFile

		
ContinueWriteFile:*/
			ADD ESP, 0x4
			TEST EAX, EAX
			JE ASSERT1 //;  apelat dupa uncompress, reg: ESI + 8 poate ?
			PUSH EBP
			CALL Call7
			ADD ESP, 0x4
			PUSH ESI
			CALL Call7
			ADD ESP, 0x4
			JMP RETURN1

RETURN1:
			XOR EAX, EAX
			POP EDI
			POP ESI
			POP EBP
			POP EBX
			ADD ESP, 0xC
			RETN


ASSERT1:
			MOV EAX, DWORD PTR SS : [ESP + 0x14]
			CMP EAX, DWORD PTR SS : [ESP + 0x10]
			JE ASSERT1_CONTINUE
			PUSH 0x217
			PUSH 0x00DDCCD0 //;  UNICODE ".\EtFileSystem.cpp"
			PUSH 0x00DDCCF8 //;  UNICODE "dwOrigin == dwOriginalSize" < -- - la asserturi se dau de gol : ))
			CALL DWORD PTR DS : [0xE80E70] // e ok;  dragonne.00AC0340
			ADD ESP, 0xC
			JMP ASSERT1_CONTINUE

ASSERT1_CONTINUE:
			PUSH EBP
			CALL Call7
			ADD ESP, 0x4
			PUSH 0x148
			CALL Call8
			ADD ESP, 0x4
			TEST EAX, EAX
			JE ASSERT1_CONTINUE2
			MOV ESI, EAX
			CALL Call9
			JMP ASSERT1_CONTINUE3

ASSERT1_CONTINUE2:
			XOR EAX, EAX
			JMP ASSERT1_CONTINUE3

ASSERT1_CONTINUE3:
			MOV ESI, EBX
			LEA EDI, DWORD PTR DS : [EAX + 0x4]
			MOV ECX, 0x4F
			REP MOVS DWORD PTR ES : [EDI], DWORD PTR DS : [ESI]
			MOV ECX, DWORD PTR SS : [ESP + 0x18]
			POP EDI
			POP ESI
			POP EBP
			MOV DWORD PTR DS : [EAX + 0x140], ECX
			MOV DWORD PTR DS : [EAX + 0x144], 0x0
			POP EBX
			ADD ESP, 0xC
			RETN
	}
}


void zlibHook()
{
	SetRange((LPVOID)0x00BE6650, 0x133, ASM::NOP); // 00BE6650  /$ 83EC 0C        SUB ESP,0xC                              ;  zlib uncompress
	SetOp((LPVOID)0x00BE6650, (LPVOID)etOpenFile, ASM::JMP);
}

