// ================================================================= //
// # LinkMania Hack For MuOnline	                               # //
// # Project Started Mecanik1337 and Alin1337                      # //
// # This file is part of the LinkMania Hack Project               # //
// # www.Addicted2.ro || Ralukat Labs                              # //
// ================================================================= //
#include <Windows.h>
#include "Detours.h"
#include "se.h"



// -------------------------------------------------------------------
// Functie pentru detour...
void DetourAndErrorCheck(PVOID* pFunctionToDetour, PVOID pReplacement, const char* functionName)
{
	char* debugString = new char[256];
	ZeroMemory(debugString, 256);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(pFunctionToDetour, pReplacement);
	int errorCode = DetourTransactionCommit();
	if (errorCode == NO_ERROR)
	{

	}
	else
	{
		if (errorCode == ERROR_INVALID_DATA)
			MessageBoxA(NULL, enc("ERROR_INVALID_DATA returned"), enc("ERROR"), MB_OK);
		else if (errorCode == ERROR_INVALID_OPERATION)
			MessageBoxA(NULL, enc("ERROR_INVALID_OPERATION returned"), enc("ERROR"), MB_OK);
		else

			OutputDebugString(debugString);
	}
	delete[] debugString;
}

// -------------------------------------------------------------------
// Functie pentru detour... gasire functie
void CheckAndDetach(PVOID* pDetouredFunction, PVOID pReplacement, const char* functionName)
{
	if (pDetouredFunction != NULL)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)pDetouredFunction, pReplacement);
		DetourTransactionCommit();
	}
	else
	{
		int errorSize = 64 + strlen(functionName);
		char* errorString = new char[errorSize];
		ZeroMemory(errorString, 32);
		MessageBoxA(NULL, enc(" could not be detached\n Was it detoured in the first place?"), "ERROR", MB_OK);
		OutputDebugString(errorString);
		delete[] errorString;
	}
}