#include <Windows.h>
#include <string>
#include <vector>
#include "GameValidation.h"
#include "se.h"
#include "Memory.h"
#include "Security.h"
#include "Metrics.h"
#include "Fixes.h"
#include "GameObject.h"

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
		//enc("127.0.0.1"),
		enc("172.168.254.13"),
		//enc("89.39.13.247"),
#endif
#ifdef RO
		enc("136.243.177.187"),
		enc("31.186.251.186"),
		enc("31.186.251.198"),
		enc("31.186.251.80"),
		enc("172.16.251.10"),
		enc("172.16.251.20"),
		enc("172.16.251.30"),
#endif
#ifdef RU
	/*	enc("185.10.61.153"),
		enc("185.10.62.192"),
		enc("5.167.50.25"),
		enc("89.39.13.247"),
		enc("193.124.119.153"),
		enc("194.58.69.192")
*/		enc("136.243.177.186")
#endif 
#ifdef AR
		enc("188.165.44.175"),
		enc("51.254.49.41"),
		enc("138.201.160.157")
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
		enc("220.170.89.151"),
#ifdef DEBUG
		enc("192.168.0.183")
#endif
#endif
#ifdef THA
		enc("43.254.132.10"), //yelp ?
		enc("43.254.133.65"), //easy
		enc("43.254.133.143")
#endif
#ifdef CHN_SRC
		enc("124.228.91.142"),
		enc("27.254.155.10")
#ifdef DEBUG
		enc("192.168.1.100"),
#endif
#endif
#ifdef CHN_TNT
		enc("222.186.3.52"),
		//enc("124.160.134.30"),
		//enc("222.186.129.6"),
#ifdef DEBUG
		enc("192.168.1.111"),
#endif
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
	//char *ip = (char*)0x0018ECF0;
	
	char *ip = getIPPointer();


	//0x0018ecd8 + 0x18
	return ip;
}

int init = 0;
DWORD WINAPI CheckServerThread(LPVOID)
{
	Sleep(60000);
	char *ip = getIP();
	if (checkIP(ip) == true)
	{
		if (init == 0)
		{
			init = 1;
			//LoadAllFixes();
			CreateThread(NULL, 0, MetricsCollectThread, NULL, NULL, 0);
		}
	}else{

#ifdef DEBUG
		MessageBox(NULL,"Game Cannot RUN","NOT OK",MB_OK);
#endif
		s.crash();
		ExitProcess(0);
	}
	return 0;
}

DWORD WINAPI MetrixThread(LPVOID)
{
	Sleep(60000);
	char *ip = getIP();
	
	if (init == 0)
	{
		init = 1;
		CreateThread(NULL, 0, MetricsCollectThread, NULL, NULL, 0);
	}
	
	return 0;
}