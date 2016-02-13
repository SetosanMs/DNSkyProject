#include <windows.h>
#include <stdio.h>
#include "SmartPointers.h"
#include "GameObject.h"
#include "Memory.h"

//globale
PlayerDataStruct PlayerData;

#define dPOINTER(Offset,DataAt) ((DataAt) + Offset)

DWORD getPointer(const DWORD Pointer,const DWORD Offset)
{
	DWORD* BaseAddress = (DWORD*)GetModuleHandle(NULL);
	DWORD* address = (DWORD*)((char*)BaseAddress + Pointer);
	address = (DWORD*)((char*)*address + Offset);
	DWORD result = *(DWORD*)address;
	return result;
}


DWORD getMap()
{

	DWORD MapID = getPointer(0x12d1de0,0xA4);
	return MapID;

	/*
	void* pointer= (DWORD*)(0x16D1DE0) + 0xA4;

	DWORD mp = NULL;
	DWORD MapID = GetDword((LPVOID)pointer, mp);

	return mp;

	//LPVOID mapid = 0x16D1DE0;// +0xA4;// (int *)dPOINTER(0x16d1de0, 0xa4);// (int*)(0x16D1DE0) + 0xA4;
	DWORD MapPTR = NULL;
	MapPTR = *(DWORD*)(MapPTR)+0xA4;
	
	DWORD* MapID = 0;
	ReadMemory((LPVOID)MapPTR, &MapID,4);


	printf("MAP ADDR: %X %X %d %d %d\n", &MapPTR, MapPTR, (DWORD*)MapPTR + 0xA4, MapPTR + 0xA4, *(DWORD*)(MapPTR) + 0xA4);

	DWORD dwBuffer = 0; // Temporary storage for memory copied from basepointer address.

	return MapPTR+0xA4;
	*/

	
	//	*mapid = 0x16D1DE0;
//	mapid+ = 0xA4;

	//return mapid;
}

void LoadGameObject()
{
CONST DWORD dwCameraDirectionOfs = 0xBFC;

CONST DWORD dwCharBase = FindPointerAddressPattern((unsigned char*)CharBasePattern, "[[[[xxxxxx[[[[xx[[[[xx[[[[xxxx[[[[xx[", 0, 0x00400000);
CONST DWORD dwCameraBase = FindPointerAddressPattern((unsigned char*)CameraBasePattern, "[[[[xxxxx[[[[xxxxxxx", 0, 0x00400000);
CONST DWORD dwMapIndex = FindPointerAddressPattern((unsigned char*)MapBasePattern, "[[[[xx[xxxxx[xxxxxxx", 0, 0x00400000);

CONST DWORD dwGoldCountAddy = FindPointerAddressPattern((unsigned char*)GoldBasePattern, "[[[[xxxxxxxxxx", 0, 0x00400000);//00 00 A1 [[ [[ [[ [[ 8B 3C 06 85 FF 74 7B 6A 00 68
CONST DWORD dwGoldCountOfs = 0x68;

CONST DWORD dwClickObject = FindPattern((unsigned char*)ClickChestPattern, "xxxxxxx[[[[xxxxxxxxxxxxxxxx", 0, 0x00500000); //81 EC 88 00 00 00 A1 [[ [[ [[ [[ 33 C4 89 84 24 84 00 00 00 8B 84 24 8C 00 00 00

CONST DWORD dwRemoveIntroMovies = FindPattern((unsigned char*)RemoveIntroMoviePattern, "xxxxxxx[[[[xxxxx[[[[", 0, 0x00A50000); //81 EC 10 02 00 00 A1 [[ [[ [[ [[ 33 C4 89 84 24 [[ [[ [[ [[


printf("CharBase : %X \n", dwCharBase);
printf("Map : %d  %X\n", getMap(), dwMapIndex);
printf("Camera : %X \n", dwCameraBase);
printf("dwGoldCountAddy: %X value: %d \n", dwGoldCountAddy, ((DWORD*)dwGoldCountAddy) + dwGoldCountOfs);
printf("dwRemoveIntroMovies: %X value: %X \n", dwRemoveIntroMovies, *(DWORD*)dwRemoveIntroMovies);
}

