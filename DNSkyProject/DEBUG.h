#include "CharCreate.h"
#include "MemoryString.h"
#include "zLibUncompress.h"
#include "SendFunction.h"
#include "GameValidation.h"
void LOAD_DEBUG_SWITCHES()
{
	//Added 11.11.2015 by Alin
	//Trying to fix Machina+Lencea select character.
	CharCreateFix();
	WriteStringsFix();
	//Added 07.02.2016 by Alin
	//Securing zlib uncompress.
	zlibHook();
	//12.02.2016 Securing packet send function (virtualizing it)
	HookSendFunction();
	//13.02.2016 LoadList
	LoadAllowList();
}