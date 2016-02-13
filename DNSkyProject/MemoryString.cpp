#include <Windows.h>
#include "MemoryString.h"
#include "Memory.h"
#include "se.h"

void WriteStringsFix()
{
	//SKILLS
	const char * lencea_skill_1st = enc("lencea_skill_1st_piercer.act");
	const char * lencea_skill_2st1 = enc("lencea_skill_2st_flurry.act");
	const char * lencea_skill_2st2 = enc("lencea_skill_2st_stingbreezer.act");
	const char * darkavenger_skill_1st = enc("warrior_skill_1st_avenger.act");
	const char * darkavenger_skill_2nd = enc("warrior_skill_2nd_darkavenger.act");

	const char * machina_skill_1st = enc("machina_skill_1st_patrona.act");
	const char * machina_skill_2st1 = enc("machina_skill_2st_defensio.act");
	const char * machina_skill_2st2 = enc("machina_skill_2st_ruina.act");
	//Create Character
	const char * lenceaCreateButton1 = enc("ID_RBT_ASTA");
	const char * lenceaCreateButton2 = enc("ID_STATIC_LENCEA");
	const char * lenceaCreateIcon = enc("ID_ICON_LENCEA");

	const char * machinaCreateButton1 = enc("ID_RBT_MA");
	const char * machinaCreateButton2 = enc("ID_STATIC_MACHINA");
	const char * machinaCreateIcon = enc("ID_ICON_MACHINA");


	const char * avengerCreateButton2 = enc("ID_STATIC_AVENGER");

	//Lencea string
	const char * lencea = enc("Lencea");
	const char * lencealower = enc("lencea");
	const char * lenceaupper = enc("LENCEA");

	const char * machina = enc("Machina");
	const char * machinalower = enc("machina");
	const char * machinaupper = enc("MACHINA");

	//lencea
	WriteMemory((PVOID)0x01F21651, (LPVOID)lencea_skill_1st, strlen(lencea_skill_1st));
	WriteMemory((PVOID)0x01F21671, (LPVOID)lencea_skill_2st1, strlen(lencea_skill_2st1));
	WriteMemory((PVOID)0x01F21691, (LPVOID)lencea_skill_2st2, strlen(lencea_skill_2st2));// 1F21691

	//dark avenger
	WriteMemory((PVOID)0x01F21820, (LPVOID)darkavenger_skill_1st, strlen(darkavenger_skill_1st));
	WriteMemory((PVOID)0x01F21840, (LPVOID)darkavenger_skill_2nd, strlen(darkavenger_skill_2nd));

	//machina
	WriteMemory((PVOID)0x01F21710, (LPVOID)machina_skill_1st, strlen(machina_skill_1st));// 
	WriteMemory((PVOID)0x01F21730, (LPVOID)machina_skill_2st1, strlen(machina_skill_2st1));// 
	WriteMemory((PVOID)0x01F21750, (LPVOID)machina_skill_2st2, strlen(machina_skill_2st2));// 

	//Lencea Create Button
	WriteMemory((PVOID)0x01F22700, (LPVOID)lenceaCreateButton1, strlen(lenceaCreateButton1));
	WriteMemory((PVOID)0x01F22720, (LPVOID)lenceaCreateButton2, strlen(lenceaCreateButton2));
	WriteMemory((PVOID)0x01F22780, (LPVOID)lenceaCreateIcon, strlen(lenceaCreateIcon));

	//Add lencea missing somewhere??
	WriteMemory((PVOID)0x01F22740, (LPVOID)lencea, strlen(lencea));
	WriteMemory((PVOID)0x01F227A0, (LPVOID)machina, strlen(machina));

	//machina
	WriteMemory((PVOID)0x01F22760, (LPVOID)lencealower, strlen(lencealower));
	WriteMemory((PVOID)0x01F227C0, (LPVOID)machinalower, strlen(machinalower));

	WriteMemory((PVOID)0x01F22820, (LPVOID)lenceaupper, strlen(lenceaupper));
	WriteMemory((PVOID)0x01F22840, (LPVOID)machinaupper, strlen(machinaupper));


	WriteMemory((PVOID)0x01F22860, (LPVOID)machinaCreateButton1, strlen(machinaCreateButton1));
	WriteMemory((PVOID)0x01F227D0, (LPVOID)machinaCreateButton2, strlen(machinaCreateButton2));
	WriteMemory((PVOID)0x01F228A0, (LPVOID)machinaCreateIcon, strlen(machinaCreateIcon));

	//avenger
	WriteMemory((PVOID)0x01F22880, (LPVOID)avengerCreateButton2, strlen(avengerCreateButton2));
}