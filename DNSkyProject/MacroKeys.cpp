#include <Windows.h>
#include "Memory.h"

void MacroKeysFix()
{
	//Write 9clasess
	SetByte((PVOID)0x00514A13, 0x09);
	
	//increase buffer to 0x12C
	SetByte((PVOID)0x00514A58, 0x2C);
	SetByte((PVOID)0x00514A59, 0x01);
	SetByte((PVOID)0x00514AF2, 0x2C);
	SetByte((PVOID)0x00514AF3, 0x01);
	
	//write 0051574B   EB 74            JMP SHORT dragon3x.005157C1
	
	BYTE FixMacroKeys[] = { 0xEB, 0x74, 0x90 };
	WriteMemory((LPVOID)0x0051574B, FixMacroKeys, sizeof(FixMacroKeys));
}