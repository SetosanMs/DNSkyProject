extern char autostart[255];

DWORD WINAPI InitRadioThread(LPVOID);
DWORD WINAPI HideRadioThread(LPVOID);
DWORD WINAPI RadioAutoStart(LPVOID);
void ShowRadio();
void HideRadio();
void PlayRadio();
void PauseRadio();
void StopRadio();
void RadioSetVolume(int vol);

void GetCurrentSong();

extern HINSTANCE gHInst;