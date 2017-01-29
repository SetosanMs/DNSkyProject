#include <Windows.h>
#include "dconnect.h"
#include "vlizer.h"
#include "se.h"
#include "Security.h"
#include <vector>
#include <string>
#include <map>
#include "Fixes.h"
#include <VMProtectSDK.h>
#include "GameValidation.h"
#include "Metrics.h"

//variabile globale
bool ValidIP = false;
bool hooked = false;
boolean Allocated = FALSE;
std::string AllocatedIP;
int metrixSent = 0;
//detour
int (WINAPI *dconnect)(SOCKET, const struct sockaddr*, int) = NULL;
int WINAPI myconnect(SOCKET s, const struct sockaddr *name, int namelen);


int WINAPI myconnect(SOCKET sock, const struct sockaddr *name, int namelen) {
	
	////VMProtectBeginVirtualization("Detour Connect");
	SOCKADDR_IN* name_in = (SOCKADDR_IN*)name;
	unsigned short uPort = ntohs(name_in->sin_port);
	char *sender = inet_ntoa(name_in->sin_addr);

	if (uPort == 80 || uPort == 8014)
	{
		return dconnect(sock, name, namelen);
	}

	if (strcmp(sender, enc("127.0.0.1")) == 0 || strcmp(sender, enc("211.218.230.106")) == 0 || strcmp(sender, enc("124.158.12.181")) == 0){
		return dconnect(sock, name, namelen);
	}


	//METRIX
#if defined(METRIX)
	if (metrixSent == 0) 
	{
		metrixSent = 1;
		//CreateThread(NULL, 0, MetricsCollectThread, NULL, NULL, 0);
	}
#endif

	AllocatedIP = sender;


#ifndef RO

#if !defined(RO) || defined(DEBUG) && !defined(RD) 
	AllocatedIP = sender;
#endif

#if defined(RO) && !defined(DEBUG) || defined(RD) 
	std::map<std::string, std::string> IPList;
	IPList["172.16.251.10"] = enc("31.186.251.198");
	IPList["172.16.251.20"] = enc("31.186.251.186");
	IPList["172.16.251.30"] = enc("31.186.251.80");
	//IPList["149.202.144.251"] = enc("149.202.144.251");

	if (Allocated == FALSE)
	{
		if (IPList.find(sender) == IPList.end())
		{
			char buff[128];
			wsprintf(buff, enc("Failed to allocate ip stack.\n\nContact SkyDN Support."));
			MessageBoxA(NULL, buff, enc("CRITICAL ERROR"), MB_OK);
			Sleep(5000);
			//security s;
			s.crash();
			ExitProcess(0);
		}
	}

#ifdef DEBUG
	if (Allocated == TRUE)
	{
		printf(enc("Connect : %s:%d -> Allocation : %s \n"), sender, uPort, AllocatedIP.c_str());
	}
	else
	{
		printf(enc("Connect : %s:%d -> First Allocation : %s \n"), sender, uPort, IPList.find(sender)->second.c_str());
	}
#endif

	sockaddr_in newConnection;
	newConnection.sin_family = name_in->sin_family;
	if (uPort == 27015 || uPort == 44400)
	{
		if (Allocated == TRUE)
		{
			newConnection.sin_addr.s_addr = inet_addr(AllocatedIP.c_str());
		}
		else
		{
			Allocated = TRUE;
			AllocatedIP = IPList.find(sender)->second.c_str();
			newConnection.sin_addr.s_addr = inet_addr(AllocatedIP.c_str()); //na alocatie
		}
	}
	else //else GAMESERVER ??
	{
		newConnection.sin_addr = name_in->sin_addr;
	}
	newConnection.sin_port = name_in->sin_port;
#endif

#endif




	//VIRTUALIZER_DOLPHIN_WHITE_START
	if (!hooked)
	{
		for (DWORD i = 0; i < AllowList.size(); i++)
		{
			if (strcmp(AllocatedIP.c_str(), AllowList[i].c_str()) == 0)
			{
				ValidIP = TRUE;
			}
		}

		if (!hooked && ValidIP == TRUE)
		{
			LoadAllFixes();
			hooked = TRUE;
		}
		else
		{
			s.crash();
			ExitProcess(0);
		}
	}

	//VIRTUALIZER_DOLPHIN_WHITE_END
	/*
#if defined(RO) && !defined(DEBUG) || defined(RD) 
		return dconnect(sock, (SOCKADDR*)&newConnection, namelen);
#else
#endif
	*/
	return dconnect(sock, name, namelen);

	//VMProtectEnd();
}