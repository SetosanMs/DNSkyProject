
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <Lmcons.h>
#include <dirent.h>
#include "../DNPakCrypto/DNPakCrypto/HTTPClient.h"
#include "Metrics.h"
#include "Extras.h"

Metrics::Metrics()
{
}


Metrics::~Metrics()
{
}

std::string string_to_hex(const std::wstring& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const wchar_t c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}


int StringToWString(std::wstring &ws, const std::string &s)
{
	std::wstring wsTmp(s.begin(), s.end());

	ws = wsTmp;

	return 0;
}

/*
  Antihack thread to send statistics.
	Folosit pentru prevenirea hackurilor ? meh
*/
DWORD WINAPI MetricsCollectThread(LPVOID lpParam)
{
	std::wstring str;

	//Get LOADED MODULES!
	str.append(L"Loaded Modules:\n");
	MODULEENTRY32W pe32;
	pe32.dwSize = sizeof(MODULEENTRY32W);
	HANDLE hSnapshotStart = NULL;
	hSnapshotStart = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (Module32FirstW(hSnapshotStart, &pe32)){
		do{
			str.append(pe32.szExePath);
			str.append(L"\n");

		} while (Module32NextW(hSnapshotStart, &pe32));
	}
	CloseHandle(hSnapshotStart);

	//Get PROCESS LIST!
	str.append(L"\nProcesses:\n");
	PROCESSENTRY32W proc;
	HANDLE hSnapshot = NULL;

	proc.dwSize = sizeof(PROCESSENTRY32W);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32FirstW(hSnapshot, &proc)){
		do{
			str.append(proc.szExeFile);
			str.append(L"\n");
		} while (Process32NextW(hSnapshot, &proc));
	}
	CloseHandle(hSnapshot);

	//Get WINDOW NAMES!
	// TO BE DONE

	//Get LOADED PAKs!

	DIR *dir = opendir("./"); //current dir
	if (!dir)
	{
		goto skip_pak;
	}

	str.append(L"\nLOADED Paks:\n");
	dirent *entry;
	while (entry = readdir(dir))
	{
		if (has_suffix(stolower(entry->d_name), ".pak"))
		{
			std::wstring pakname;
			StringToWString(pakname, entry->d_name);
			str.append(pakname);
			str.append(L"\n");
		}
	}
	goto skip_pak;

skip_pak:
	closedir(dir);

	//bagai new line.
	str.append(L"\n");

	//Get Username!
	wchar_t username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameW(username, &username_len);
	str.append(L"User:");
	str.append(username);
	str.append(L"\n");

	//Get PC NAME
	wchar_t nameBuf[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD nameBufSize;

	nameBufSize = sizeof nameBuf - 1;
	if (GetComputerNameW(nameBuf, &nameBufSize) == TRUE) {
		str.append(L"PC NAME:");
		str.append(nameBuf);
		str.append(L"\n");
	}
	//crypt string.
	//to be done sau ?? lenea..

	//send data.
	gHTTP.SendCollectorData(eRequest::POST, string_to_hex(str), str.size()*sizeof(wchar_t));

	return NULL;
}
