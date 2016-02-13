#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <dirent.h>
#include <conio.h>
#include "..\\..\\DNSkyProject\\vlizer.h"
#include "Header.h"

using namespace std;

bool cryptpakv3(const string& s) {
	string outputF = "encrypted_pak\\" + s;
	std::ifstream infile(s, std::ifstream::binary);

	unsigned char listpointer[4];
	int listdata;

	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);



		//First crypt DATA
		SPackingFileHeader *header = (SPackingFileHeader*)buffer;
		printf("File count : %d \n",header->nFileCount);


		int filesCrypted=0;
		for (int i = 0; i < header->nFileCount; i++)
		{
			SPackingFileInfo * file = (SPackingFileInfo*)&buffer[header->dwFileInfoOffset + (sizeof(SPackingFileInfo)*i)];
			
			int j = 1;
			for (DWORD i = file->dwOffset + 1; i < (file->dwOffset + file->dwCompressSize); i++)
			{
				buffer[i] ^= cheimagice[j % 512];
				j++;
			}
			j = 1;
			filesCrypted++;
			//	printf("Processing File : %s\n",file->szFileName);
		}

		printf("%s  - > Crypted success %d files of total %d files!\n",s.c_str(), filesCrypted , header->nFileCount);

		//HERE HEADER
		char *RaluKat_Stamp = "Ralukat Labs Packing System 1.1";
		*RaluKat_Stamp = '\0';

		for (DWORD i = 0; i < strlen(RaluKat_Stamp); i++)
		{
			buffer[i] = RaluKat_Stamp[i];
		}

		for (int i = strlen(RaluKat_Stamp); i < 0x110; i++)
		{
			if (i == 0x108)
			{
				for (int j = 0; j < 4; j++)
					listpointer[j] = buffer[i + j];

				listdata = listpointer[3] << 24 | listpointer[2] << 16 | listpointer[1] << 8 | listpointer[0];
#ifdef DEBUG	
				printf("Filelist At: %X \n", listdata);
#endif
			}
			//Data[i] ^= Data[i - 1] ^ cheimagice[i % 32];

			buffer[i] ^= buffer[i - 1] ^ cheimagice[i % 512];
		}


		//more fuck
		char rlkt[] = "RLKT ";
		for (int i = 0x110; i < 0x400; i++)
		{
			buffer[i] ^= rlkt[i % (sizeof(rlkt)-1)];
		}



		//FILELIST!!
		//lets fuck it :D
		//Crypt 1 
		int j = 0;
		for (int i = listdata; i < listdata + 0x32; i++)
		{
			buffer[i] ^= cheimagice[(j + 0x69) % 512]; //fuck yea?
			j++;
		}

		//CRYPT 2 with advanced xoring technique :)

		j = 0x32;
		for (int i = listdata + 0x32; i < size; i++)
		{
			buffer[i] ^= buffer[i - 1] ^ cheimagice[j % 512];
			j++;
		}
#ifdef DEBUG	
		printf("Last NUMBER = %X %x %x %x \n", buffer[size], buffer[size - 1], buffer[size - 2], buffer[size - 3]);
#endif
		
		//infile.seekg(0, ios::beg);
		//cout << "File : " << s << " size: " << size << endl;
		
		
		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);
		outfile.close();
		
		
		infile.close();


		free(buffer);

	}

	return true;

}
