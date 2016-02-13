#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ostream>
#include <string>
#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>
#include <vector>
#include <sstream>
#include "se.h"
#include "Extras.h"

std::string getFileName(std::string input)
{
	int contor = input.find_last_of('\\');
	if (contor != std::string::npos)
		input = input.substr(contor + 1); // f contains the result :)
	return input;
}

int getClientVer()
{
	int cVer = 0;
	std::ifstream version;
	version.open(enc("Version.cfg"));
	if (version.is_open())
	{
		std::string line;
		std::getline(version, line);
		sscanf_s(line.c_str(), enc("Version %d"), &cVer);
	}
	else{
		cVer = 1;
	}
	version.close();
	return cVer;
}

void print(char* mesaj)
{
	printf(enc("%s\n"), mesaj);
}

bool FileExists(std::string path)
{
	std::ifstream my_file(path);
	if (my_file)
	{
		return true;
	}
	return false;
}

std::string capital(std::string name)
{

	for (DWORD i = 0; i < name.length(); i++)
	{
		name[i] = toupper(name[i]);
	}

	return name;

}
std::string stolower(std::string name)
{
	for (DWORD i = 0; i < name.length(); i++)
	{
		name[i] = tolower(name[i]);
	}

	return name;

}
const char *GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	TCHAR pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
#ifdef DEBUG
		_tprintf(TEXT("Cannot map a file with a length of zero.\n"));
#endif
		return "C:\\matsukata.dll";
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		1,
		NULL);

	if (hFileMap)
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(),
				pMem,
				pszFilename,
				MAX_PATH))
			{

				// Translate path with device name to drive letters.
				TCHAR szTemp[512];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(512 - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
									&& *(pszFilename + uNameLen) == _T('\\');

								if (bFound)
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									StringCchPrintf(szTempFile,
										MAX_PATH,
										TEXT("%s%s"),
										szDrive,
										pszFilename + uNameLen);
									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}
	//_tprintf(TEXT("File name is %s\n"), pszFilename);
	return "C:\\";
	//return(bSuccess);
}

bool has_suffix(const std::string& s, const  std::string& suffix)
{
	return (s.size() >= suffix.size()) && std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

