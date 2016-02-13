#include <windows.h>


void Main()
{

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, PVOID lpReserved)
{
	//VIRTUALIZER_DOLPHIN_WHITE_START
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		Main();
		break;


	case DLL_PROCESS_DETACH:
		
		break;
	}
	//VIRTUALIZER_DOLPHIN_WHITE_END

	return TRUE;
}