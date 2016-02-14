#include <Windows.h>
#include <string>
#include <vector>
#include "GameValidation.h"
#include "se.h"
#include "Memory.h"
#include "Security.h"
#include "Metrics.h"
#include "Fixes.h"

std::vector<std::string> AllowList;

GameValidation::GameValidation()
{
}


GameValidation::~GameValidation()
{
}

void LoadAllowList()
{
	 AllowList = {
#ifdef DEBUG
		enc("127.0.0.1"),
		enc("172.168.254.3"),
		enc("89.39.13.247"),
#endif
#ifdef RO
		enc("149.202.144.250"),
		enc("149.202.144.251"),
		enc("31.186.251.186"),
		enc("31.186.251.198"),
		enc("31.186.251.80"),
#endif
#ifdef RU
		enc("185.10.61.153"),
		enc("185.10.62.192"),
		enc("5.167.50.25"),
		enc("89.39.13.247"),
		enc("193.124.119.153"),
		enc("194.58.69.192")
#endif 
#ifdef AR
		enc("188.165.44.175"),
		enc("51.254.49.41")
#endif
#ifdef VTM
		enc("124.158.12.182"),
		enc("124.158.12.183")
#endif
#ifdef CA
		enc("23.250.112.202")
#endif
#ifdef CHN
		enc("172.168.254.3"),
		enc("117.27.248.30"),
		enc("36.250.231.32")
#endif
#ifdef CHN_MSTERE
		enc("220.169.242.205"),
#ifdef DEBUG
		enc("192.168.0.183")
#endif
#endif
#ifdef THA
		enc("203.150.228.195")
#endif
	};
}

bool checkIP(std::string ip){
	
	for (DWORD i = 0; i < AllowList.size(); i++)
	{
		if (strcmp(ip.c_str(), AllowList[i].c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}

bool checkIP(std::wstring wip){
	
	std::string ip(wip.begin(), wip.end());

	for (DWORD i = 0; i < AllowList.size(); i++)
	{
		if (strcmp(ip.c_str(), AllowList[i].c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}


char* getIP()
{
	//0018ECF0
	char *ip = (char*)0x0018ECF0;

	return ip;
}

int init = 0;
DWORD WINAPI CheckServerThread(LPVOID)
{
	Sleep(30000);
	char *ip = getIP();
	if (checkIP(ip) == true)
	{
		if (init == 0)
		{
			init = 1;
			LoadAllFixes();
			CreateThread(NULL, 0, MetricsCollectThread, NULL, NULL, 0);
		}
	}else{
		MessageBox(NULL,"Game Cannot RUN","NOT OK",MB_OK);
		s.crash();
		ExitProcess(0);
	}
	return 0;
}