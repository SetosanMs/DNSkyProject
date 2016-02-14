#include "Craft.h"
#include "CharCreate.h"
#include "ItemLinkManager.h"
#include "ClassInit.h"
#include "MemoryString.h"
#include "MacroKeys.h"
#include "ActionManager.h"
#include "EmitterEffect.h"
#include "CPUReducer.h"
#include "DNCommandSend.h"

void LoadAllFixes()
{
	WriteStringsFix();
	CraftFix();
	CharCreateFix();
	ItemLinkFix();
	ClassInitFix();
	MacroKeysFix();
	
#if !defined(CA)
#if !defined(AR)
	ActionManagerFix();
	EmitterVersionFix();
#endif
#endif


	LoadCPUReducer();
	LoadCommandProcessor();
}