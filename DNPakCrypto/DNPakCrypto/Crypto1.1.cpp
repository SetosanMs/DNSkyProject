#include <stdio.h>
#include <windows.h>
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



bool decrypt_pakv3(const string& s) {
	string outputF = "decrypted_pak\\" + s;
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



		char stampila[0x20]; //32baiti baito :P
		strncpy(stampila, ((const char*)buffer), 0xC);
		stampila[0xC] = '\0';

		//HERE
		char *RaluKat_Stamp = "Ralukat Labs Packing System 1.0";
		*RaluKat_Stamp = '\0';

		if (strcmp(stampila, "Ralukat Labs") == 0)
		{
			for (int i = 0x110; i != 0x20 - 1; i += -1)
			{
				buffer[i] ^= buffer[i - 1] ^ cheimagice[i % 512]; //512 keys
			}

			int FilelistOffset = 0;
			FilelistOffset = buffer[0x10B] << 24 | buffer[0x10A] << 16 | buffer[0x109] << 8 | buffer[0x108];
#ifdef DEBUG	
			printf("FL Offset : %X\n", FilelistOffset);
#endif

			int FileNum = 0;
			FileNum = buffer[0x107] << 24 | buffer[0x106] << 16 | buffer[0x105] << 8 | buffer[0x104];

			int newsize = FileNum*sizeof(SPackingFileInfo);
#ifdef DEBUG
			printf("FL Offset : %X readsize : %X size : %X fileoffset : %X\n", FilelistOffset, newsize, size, FilelistOffset);
#endif


			int tempsize = size;
			for (int i = size - 1; i != FilelistOffset + 0x31; i += -1)
			{
				newsize--;
				buffer[i] ^= buffer[i - 1] ^ cheimagice[newsize % 512]; //512 keys
			}
#ifdef DEBUG
			printf(" LAST NUMBERS %.2X %.2X %.2X %.2X %.2X \n ", buffer[size - 1], buffer[size - 2], buffer[size - 3], buffer[size - 4], buffer[size - 5]);

#endif
			int	j = 0;
			for (int i = FilelistOffset; i < FilelistOffset + 0x32; i++)
			{
				buffer[i] ^= cheimagice[(j + 0x69) % 512];
				j++;
			}
		}
		else{
			cout << s << " is not encrypted!!" << endl;
			infile.close();
			free(buffer);
			return false;
		}

		/*
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
		}*/

		//printf("Last NUMBER = %X %x %x %x \n", buffer[size], buffer[size - 1], buffer[size - 2], buffer[size - 3]);

		/*
		buffer[0] = 0xDA; //its crypted.:)
		for (int i = 1; i < size; i++)
		{
		buffer[i] ^= buffer[i - 1] ^ cheimagice[i % 512];
		}*/

		//infile.seekg(0, ios::beg);
		//cout << "File : " << s << " size: " << size << endl;
		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);

		infile.close();
		outfile.close();
		free(buffer);
	}

	return true;

}