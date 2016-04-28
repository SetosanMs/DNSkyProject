#include <Windows.h>
#include <string>
#include "DNCommandProcessor.h"
#include "DNCommandSend.h"
#include "GameObject.h"
#include "Radio.h"
#include "Messages.h"
#include "ArabicText.h"
#include "TextColor.h"

void nullCommand(WCHAR * cmd)
{
	cmd[0] = '/';
}

void nullCommand()
{
	commandNulled = true;
}

void ParseCommand(WCHAR * text)
{

	//Colorify(text);

#ifdef DEBUG
	int size = wcslen(text);
	for (int i = 0; i < size;i++)
		printf("%.4X ",(WORD)text[i]);

	wprintf(L"\nProcessing %ws\n",text);
#endif


#if defined(AR) 

	ArabicTextTryFix(text);

#endif



#ifdef RADIO
	//RADIO	
	if (wcscmp(L".radio", text) == 0 || wcscmp(L".r", text) == 0)
	{
		ShowRadio();
		nullCommand(text);
	}

	if (wcscmp(L".r play", text) == 0 || wcscmp(L".radio play", text) == 0)
	{
		PlayRadio();
		SendChatField(L"~~Radio~~", L"Radio is now playing!");
		nullCommand(text);
		//
	}

	if (wcscmp(L".r pause", text) == 0 || wcscmp(L".radio pause", text) == 0)
	{
		PauseRadio();
		SendChatField(L"~~Radio~~", L"Radio Paused.");
		nullCommand(text);
		//
	}

	if (wcscmp(L".r stop", text) == 0 || wcscmp(L".radio stop", text) == 0)
	{
		StopRadio();
		SendChatField(L"~~Radio~~", L"Radio Stopped.");
		nullCommand(text);
		//
	}

	if (wcscmp(L".r song", text) == 0 || wcscmp(L".radio song", text) == 0)
	{
		GetCurrentSong();
		nullCommand(text);
		//
	}

	if (wcscmp(L".r help", text) == 0 || wcscmp(L".radio help", text) == 0)
	{
		SendChatField(L"~~Radio~~", L"Radio Commands");
		SendChatField(L"~~Radio~~", L".r       -> Show Radio Interface");
		SendChatField(L"~~Radio~~", L".r play  -> Starts Radio");
		SendChatField(L"~~Radio~~", L".r pause -> Pauses Radio (use .r play to resume)");
		SendChatField(L"~~Radio~~", L".r stop  -> Stops Radio");
		SendChatField(L"~~Radio~~", L".r song  -> Displays song name");
		SendChatField(L"~~Radio~~", L".r vol   -> Sets volume (1~100)");
		nullCommand(text);
		return;
	}
	//Radio Volume
	if (wcsstr(text, L".r vol ") != NULL)
	{
		if (wcslen(text) > 7)
		{
			wchar_t * pwc;
			pwc = wcstok(text, L".r vol ");
			
			if (wcslen(text) > 0)
			{
				int vol = std::stoi(pwc);
				if (vol >= 0 && vol <= 100)
					RadioSetVolume(vol);
			}
		}

		nullCommand(text);
	}
#endif

#ifdef DEBUG
	/*if (wcscmp(L".test", text) == 0)
	{
		SendChatField(L"Server", L"-- Test1");
		SendChatGM(L"Server", L"-- Test2");
		SendChatNormal(L"Server", L"-- Test3");
		nullCommand();
	}*/

	if (wcscmp(L"rescan", text) == 0)
	{
		LoadGameObject();
		nullCommand(text);
	}
#endif

}
