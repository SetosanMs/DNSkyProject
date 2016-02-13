#include <Windows.h>
#include <stdio.h>
#include "Messages.h"

const DWORD dwPrintToChat = 0x00737490;
VOID __declspec(naked) PrintToChatNormal(wchar_t* name, wchar_t* message, int MessageType) {

	__asm {
		mov eax, [0x16D1DD0]
			mov eax, [eax]
			_emit 0x6A
			_emit 0x01 //push 01
			mov ecx, [esp + 0x0C]
			push ecx
			mov ecx, 0x1
			mov edx, [esp + 0x0C]
			push eax
			call dwPrintToChat
			ret
	}

}
VOID __declspec(naked) PrintToChatField(wchar_t* name, wchar_t* message, int MessageType) {

	__asm {
		mov eax, [0x16D1DD0]
			mov eax, [eax]
			_emit 0x6A
			_emit 0x01 //push 01
			mov ecx, [esp + 0x0C]
			push ecx
			mov ecx, 0x5
			mov edx, [esp + 0x0C]
			push eax
			call dwPrintToChat
			ret
	}

}

VOID __declspec(naked) PrintToChatGM(wchar_t* name, wchar_t* message, int MessageType) {

	__asm {
		mov eax, [0x16D1DD0]
			mov eax, [eax]
			_emit 0x6A
			_emit 0x01 //push 01
			mov ecx, [esp + 0x0C]
			push ecx
			mov ecx, 0xA
			mov edx, [esp + 0x0C]
			push eax
			call dwPrintToChat
			ret
	}

}

VOID __declspec(naked) PrintToChatNotice(wchar_t* name, wchar_t* message, int MessageType) {

	__asm {
		mov eax, [0x16D1DD0]
			mov eax, [eax]
			_emit 0x6A
			_emit 0x01 //push 01
			mov ecx, [esp + 0x0C]
			push ecx
			mov ecx, 0x8
			mov edx, [esp + 0x0C]
			push eax
			call dwPrintToChat
			ret
	}

}
VOID SendChatNormal(WCHAR *From, WCHAR *Text)
{
	PrintToChatNormal(From, Text, 0);
}

VOID SendChatField(WCHAR *From, WCHAR *Text)
{
	PrintToChatField(From, Text, 0);
}

VOID SendChatGM(WCHAR *From, WCHAR *Text)
{
	PrintToChatGM(From, Text, 0);
}

VOID SendChatNotice(WCHAR *From, WCHAR *Text)
{
	PrintToChatNotice(From, Text, 0);
}