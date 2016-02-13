#include <windows.h>
#include "HTTPGuard.h"
#include "vlizer.h"
#include <string>
#include "Base64.h"
#include "Misc.h"
#include "se.h"

void Finalize_OpenPage(char * ExecutableName)
{
	//VIRTUALIZER_DOLPHIN_WHITE_START
#ifdef AR
	const char *CraftedURL = enc("http://dnarabic.net/gameguard?d=");
#else
	const char *CraftedURL = enc("http://guard.ralukat.eu/detect/?d=");
#endif
		char FinalURL[512];

		std::string detection = AccountName+"&" + ExecutableName;
		std::string data = base64_encode((const unsigned char*)detection.c_str(), strlen(detection.c_str()));
		wsprintf(FinalURL, "%s%s", CraftedURL,data.c_str());

		//char *CraftedURL = "http://guard.ralukat.eu/detect/?d=";
		ShellExecute(NULL, "open", FinalURL, NULL, NULL, SW_SHOWNORMAL);

	//VIRTUALIZER_DOLPHIN_WHITE_END
}


