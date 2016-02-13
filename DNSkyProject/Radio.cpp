#include <Windows.h>
#include <stdio.h>
#include <process.h>
#include <string>
#include "resource.h"
#include "Radio.h"
#include "bass.h"
#include "Messages.h"
#include "se.h"
#include "GameObject.h"
#include "Extras.h"


#pragma comment(lib, "bass.lib")


HINSTANCE gHInst;
HWND win = NULL;
CRITICAL_SECTION lock;
DWORD req = 0;	// request number/counter
HSTREAM chan;	// stream handle
bool _started = FALSE;
std::wstring CurrentSong;
std::wstring LastSongPlayed;


#ifdef THA
char url[255];
#else
char *url = "http://203.150.228.195:8014/listen.pls";
//char *url = "http://stream.r-a-d.io/main.mp3";
#endif
//prototipuri
void __cdecl OpenURL(void *url);

#if defined(THA) || defined(RU)
void LoadRadioFile()
{

	if (FileExists(enc(".\\Radio.dat")))
	{
		char tempu[255];
		
		GetPrivateProfileString("RALUKAT", "URL", enc("http://stream.r-a-d.io/main.mp3"), tempu, 255, enc(".\\Radio.dat"));
		strcpy(url, tempu);

	}else{
		strcpy(url,enc("http://stream.r-a-d.io/main.mp3"));
	}

}
#endif

void GetCurrentSong()
{
	wchar_t PlayingSong[128];
	wsprintfW(PlayingSong, L"Now Playing: %s", CurrentSong.c_str());
	SendChatNotice(L"~~Radio~~", PlayingSong);
}

void ShowRadio()
{
	ShowWindow(win, SW_SHOW);
	BringWindowToTop(win);
}

void HideRadio()
{
	ShowWindow(win, SW_HIDE);
}

void PauseRadio()
{
	BASS_ChannelPause(chan);
}

void PlayRadio()
{
	if (_started)
	{
		BASS_ChannelPlay(chan, FALSE);
	}else{
		_beginthread(OpenURL, 0, url);
		
		_started = 1;
	}
}

void StopRadio()
{
	if (_started)
	{
		_started = FALSE;
		BASS_ChannelStop(chan);
	}
}

void RadioSetVolume(int vol)
{
	float media = 100.f;
	float volume = static_cast<float>(vol);

	volume = volume / media;
#ifdef DEBUG
	printf("Setez volum : %.15f %.15f \n", volume , volume/media);
#endif
	BASS_SetVolume(volume);
}
// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
	MessageBox(win, mes, 0, 0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

static wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}

// update stream title from metadata
void DoMeta()
{
	const char *meta = BASS_ChannelGetTags(chan, BASS_TAG_META);
	if (meta) { // got Shoutcast metadata
		const char *p = strstr(meta, "StreamTitle='"); // locate the title
		if (p) {
			const char *p2 = strstr(p, "';"); // locate the end of it
			if (p2) {
				char *t = _strdup(p + 13);
				t[p2 - (p + 13)] = 0;
#if !defined(THA)
				CurrentSong = charToWChar(t);
				GetCurrentSong();
#endif
				MESS(30, WM_SETTEXT, 0, t);
				free(t);
			}
		}
	}
	else {
		meta = BASS_ChannelGetTags(chan, BASS_TAG_OGG);
		if (meta) { // got Icecast/OGG tags
			const char *artist = NULL, *title = NULL, *p = meta;
			for (; *p; p += strlen(p) + 1) {
				if (!_strnicmp(p, "artist=", 7)) // found the artist
					artist = p + 7;
				if (!_strnicmp(p, "title=", 6)) // found the title
					title = p + 6;
			}
			if (title) {
				if (artist) {
					char text[100];
					_snprintf(text, sizeof(text), "%s - %s", artist, title);
#if !defined(THA)
					CurrentSong = charToWChar(text);
					GetCurrentSong();
#endif
					MESS(30, WM_SETTEXT, 0, text);
				}
				else
				{
#if !defined(THA)
					CurrentSong = charToWChar(title);
					GetCurrentSong();
#endif
					MESS(30, WM_SETTEXT, 0, title);
				}
					
			}
		}
	}
}

void CALLBACK MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	DoMeta();
}

void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	MESS(31, WM_SETTEXT, 0, "not playing");
	MESS(30, WM_SETTEXT, 0, "");
	MESS(32, WM_SETTEXT, 0, "");
}

void CALLBACK StatusProc(const void *buffer, DWORD length, void *user)
{
	if (buffer && !length && (DWORD)user == req) // got HTTP/ICY tags, and this is still the current request
		MESS(32, WM_SETTEXT, 0, buffer); // display status
}

void __cdecl OpenURL(void *url)
{
	DWORD c, r;
	EnterCriticalSection(&lock); // make sure only 1 thread at a time can do the following
	r = ++req; // increment the request counter for this request
	LeaveCriticalSection(&lock);
	KillTimer(win, 0); // stop prebuffer monitoring
	BASS_StreamFree(chan); // close old stream

	printf("[RADIO] connecting... \n");
	MESS(31, WM_SETTEXT, 0, "connecting...");
	MESS(30, WM_SETTEXT, 0, "");
	MESS(32, WM_SETTEXT, 0, "");
	c = BASS_StreamCreateURL((const char*)url, 0, BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE, StatusProc, (void*)r); // open URL
	free(url); // free temp URL buffer
	EnterCriticalSection(&lock);
	if (r != req) { // there is a newer request, discard this stream
		LeaveCriticalSection(&lock);
		if (c) BASS_StreamFree(c);
		return;
	}
	chan = c; // this is now the current stream
	LeaveCriticalSection(&lock);
	if (!chan) { // failed to open
		MESS(31, WM_SETTEXT, 0, "not playing");
#ifdef DEBUG
		printf("[RADIO] Can't play the stream... \n");
#endif
	}
	else
		SetTimer(win, 0, 50, 0); // start prebuffer monitoring
}

INT_PTR CALLBACK dialogproc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {

	case WM_TIMER:
	{ // monitor prebuffering progress
					 DWORD progress = BASS_StreamGetFilePosition(chan, BASS_FILEPOS_BUFFER)
						 * 100 / BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END); // percentage of buffer filled
					 if (progress>75 || !BASS_StreamGetFilePosition(chan, BASS_FILEPOS_CONNECTED)) { // over 75% full (or end of download)
						 KillTimer(win, 0); // finished prebuffering, stop monitoring
						 MESS(31, WM_SETTEXT, 0, "playing");
						 { // get the broadcast name and URL
							 const char *icy = BASS_ChannelGetTags(chan, BASS_TAG_ICY);
							 if (!icy) icy = BASS_ChannelGetTags(chan, BASS_TAG_HTTP); // no ICY tags, try HTTP
							 if (icy) {
								 for (; *icy; icy += strlen(icy) + 1) {
									 if (!_strnicmp(icy, "icy-name:", 9))
#ifdef DEBUG
										 printf("[RADIO]Playing : %s\n", icy + 9);
#endif
										 MESS(31, WM_SETTEXT, 0, icy + 9);
									 if (!_strnicmp(icy, "icy-url:", 8))
										 MESS(32, WM_SETTEXT, 0, icy + 8);
								 }
							 }
						 }

						 // get the stream title and set sync for subsequent titles
						 DoMeta();
						 BASS_ChannelSetSync(chan, BASS_SYNC_META, 0, &MetaSync, 0); // Shoutcast
						 BASS_ChannelSetSync(chan, BASS_SYNC_OGG_CHANGE, 0, &MetaSync, 0); // Icecast/OGG
						 // set sync for end of stream
						 BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &EndSync, 0);
						 // play it!
						 BASS_ChannelPlay(chan, FALSE);
					 }
					 else {
#ifdef DEBUG
						 char text[20];
						 sprintf(text, "buffering... %d%%", progress);
						 printf("%s\n",text);
#endif
	//					 MESS(31, WM_SETTEXT, 0, text);
					 }
	}
		break;

	case WM_COMMAND:
		switch (LOWORD(w)) {
		case IDCANCEL:
			ShowWindow(h, SW_HIDE);
			//DestroyWindow(h);
			return 1;

		case IDC_R_PAUSE:
			PauseRadio();
			break;

		case IDC_R_PLAY:
			PlayRadio();
			break;

		default:
			if ((LOWORD(w) >= 10 && LOWORD(w)<20) || LOWORD(w) == 21) {
				char *url = NULL;

				if (LOWORD(w) == 21) { // custom stream URL
					char temp[200];
					MESS(20, WM_GETTEXT, sizeof(temp), temp);
					url = _strdup(temp);
				}
				

				// open URL in a new thread (so that main thread is free)
				//_beginthread(OpenURL, 0, url);
			}
		}
		break;
		
	case WM_INITDIALOG:
		win = h;
		// initialize default output device
		if (!BASS_Init(-1, 44100, 0, win, NULL)) {
			Error("Can't initialize device");
			DestroyWindow(win);
		}
		BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
		BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0); // minimize automatic pre-buffering, so we can do it (and display it) instead
		BASS_SetConfig(BASS_CONFIG_UNICODE, 1); //unicode..

		//BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxy); // setup proxy server location
		InitializeCriticalSection(&lock);
		MESS(20, WM_SETTEXT, 0, "http://stream.r-a-d.io/main.mp3");
		
		//
		//PAUSE ICON
//		SendMessage(IDC_R_PAUSE, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)IDB_BITMAP1);
		//hbit = LoadBitmap(hInstance, L"Bit");
		//SendMessage(d, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbit);
		//PLAY ICON
		//hbit = LoadBitmap(hInstance, L"Bit");
		//SendMessage(d, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbit);
		::SendMessage(win, WM_CLOSE, NULL, NULL);
		return 1;

	case WM_DESTROY:
		BASS_Free();
		break;
	}
	return 0;
}


DWORD __stdcall InitRadioThread(LPVOID)
{

#if defined(THA) || defined(RU)
	LoadRadioFile();
#endif

	/*
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		MessageBox(0, "An incorrect version of BASS.DLL was loaded", 0, MB_ICONERROR);
		return 0;
	}

//	HWND win = GetActiveWindow();
	if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
		printf("[Radio] Can't initialize device\n");
		
	}
	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
	BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0); // minimize automatic pre-buffering, so we can do it (and display it) instead
	BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, NULL); // disable proxy
	InitializeCriticalSection(&lock);
	
	void *url = "http://www.radioparadise.com/m3u/mp3-128.m3u";

	_beginthread(OpenURL, 0, url);*/
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		MessageBox(0, "An incorrect version of BASS.DLL was loaded", 0, MB_ICONERROR);
		return 0;
	}

	// display the window
	DialogBox(gHInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, &dialogproc);


	
	return 0;
}



DWORD WINAPI HideRadioThread(LPVOID)
{
	int i = 0;
	for (i = 0; i < 10;i++)
	{
		Sleep(200);
		HideRadio();
	}
	return 0;
}


DWORD WINAPI RadioAutoStart(LPVOID)
{
	while (1)
	{
		Sleep(2000);
		if (PlayerData.isConnected == 1)
		{
			PlayRadio();
			return 0;
		}
	}
	return 0;
}