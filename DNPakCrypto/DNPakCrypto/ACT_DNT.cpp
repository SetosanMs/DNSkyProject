#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <dirent.h>
#include <conio.h>
#include <time.h>
#include "..\\..\\DNSkyProject\\vlizer.h"
#include "lzo.h"
#include "Header.h"
#include <random>
#include <stdio.h>
#include <VMProtectSDK.h>

using namespace std;




#include <vector>
using namespace std;

vector<unsigned char> intToBytes(DWORD paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

void encrypt_act(const string& s)
{
	string outputF = "encrypted_act\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		if (buffer[0x15] == 0xDA || buffer[0x15] == 0xDB)
		{
			printf("%s - > Already encrypted, skipping!\n", s.c_str());
			return;
		}
		if (buffer[0x00] != 'E' && buffer[0x01] != 't')
		{
			printf("%s - > Invalid ACT file, skipping!\n", s.c_str());
			return;
		}

		//

		
		///start!
		DWORD TotalEncryptedBytes = 0x00;
		DWORD i = 0x20;

		int key_size = 0;
		BYTE key[9] = { 0 };

		//generare potentiale chei.
		DWORD test[256] = { 0 };
		for (int i = 0x20; i < size; i++)
		{
			if (buffer[i] != 0x00)
			{
				test[buffer[i]] += 1;
			}
		}



		VMProtectBeginVirtualization("ACT Encrypt");
		//lista cheilor potentiale
		std::vector<BYTE> chei;// = new std::vector<BYTE>();
	//	printf("FILE : %s \n",s.c_str());
		for (int i = 1; i <= 255;i++)
		{
			if (test[i] == 0x00)
			{
		 //	printf("%X = %X\n",i,test[i]);
				chei.push_back(i);
				key_size++;
			}
		}

		int MarimeChei = chei.size();
		//stabilim dimensiunea fixa 9!
		if (key_size > 9)
			key_size = 9;

		if (key_size != 0 && chei.size() != 0)
		{
			//alegerea cheilor din lista
			//printf("Chei alese : ");
			for (int i = 0; i < key_size; i++)
			{
/*				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_int_distribution<int> dist(0, chei.size());
	*/
				key[i] = chei[(i*8)%chei.size()] & 0xFF; //hack
				for (int j = 0; j < chei.size(); j++)
				{
					if (chei[j] == key[i])
						chei.erase(chei.begin()+j);
				}
			//	printf("%X ",key[i]);
			}
			//printf("\n");

			//criptare
			i = 0x20; //incepem de la 0x20
			for (; i < size; i++)
			{
				if (buffer[i] != 0x00)
				{
					buffer[i] ^= key[i % key_size];
				
					TotalEncryptedBytes++;
					if (buffer[i] == 0x00)
					{
						printf("ATTENTION, NULL HOLE!!! %X , cheie %X\n", i, key[i % key_size]);
					}
				}
			}
		
			//printf("%s - > Encrypted %d Bytes, Potential Keys: %d , Total Keys: %d!\n", s.c_str(), TotalEncryptedBytes, MarimeChei, key_size);

	
			//scriem header.
			buffer[0x15] = 0xDA; //este criptatt
			buffer[0x16] = key_size; //dimensiunea la chei
			for (int i = 0; i < key_size; i++)
			{
				buffer[i + 0x17] = key[i]; //cheile
			}

			//

			//encrypt key length
			buffer[0x16] ^= buffer[0x24];
		}else{
			//
			DWORD CryptoSize = buffer[0x24] * 10;

			//encrypt header
			for (DWORD j = 0x28; j < CryptoSize; j++)
			{
				buffer[j] ^= cheimagice[j % 512];
				TotalEncryptedBytes++;
			}

			//encrypt data

			//encrypt data 8byte block!
			for (DWORD i = CryptoSize; i < size; i += CryptoSize)
			{
				for (DWORD j = 0; j < 8; j++)
				{
					buffer[i + j] ^= cheimagice[(j + 2) % 512];
				}
				TotalEncryptedBytes += 8;
			}


			//criptare STATICA PHA!
			buffer[0x15] = 0xDB; //Criptate v2


			//printf("%s - > Encrypted %d Bytes! Version 2\n", s.c_str(), TotalEncryptedBytes);

		}
			
		VMProtectEnd();
		chei.clear();


		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;

}

void decrypt_act(const string& s)
{

	string outputF = "decrypted_act\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		VMProtectBeginVirtualization("ACT DEcrypt");

		if (buffer[0x15] == 0xDA)
			goto NEXT;
		if(buffer[0x15] == 0xDB)
			goto NEXT;



		printf("%s - > Not encrypted, skipping!\n", s.c_str());
		return;

NEXT:

		if (buffer[0x00] != 'E' && buffer[0x01] != 't')
		{
			printf("%s - > Invalid ACT file, skipping!\n", s.c_str());
			return;
		}

		//globala
		DWORD TotalEncryptedBytes = 0x00;

		if (buffer[0x15] == 0xDA)
		{

			//decrypt key length
			buffer[0x16] ^= buffer[0x24];

			//get keys
			BYTE key_size = buffer[0x16];
			BYTE key[9] = { 0 };

			for (BYTE i = 0; i < key_size; i++)
			{
				key[i] = buffer[i + 0x17];
			}
			//


			///start!
			DWORD i = 0x20;
			DWORD j = 0;

			for (; i < size; i++)
			{
				if (buffer[i] != 0x00)
				{
					buffer[i] ^= key[i % key_size];
					TotalEncryptedBytes++;
				}
			}


			printf("%s - > DEcrypted %d Bytes!\n", s.c_str(), TotalEncryptedBytes);

			//HERE HEADER
			char *RaluKat_Stamp = "dRLKT13";
			*RaluKat_Stamp = '\0';

			i = 0;
			j = 0x15;
			for (; i < strlen(RaluKat_Stamp); i++)
			{
				buffer[j] = RaluKat_Stamp[i];
				j++;
			}
		}
		else if(buffer[0x15] == 0xDB)
		{
			DWORD CryptoSize = buffer[0x24] * 10;

			//encrypt header
			for (DWORD j = 0x28; j < CryptoSize; j++)
			{
				buffer[j] ^= cheimagice[j % 512];
				TotalEncryptedBytes++;
			}

			//encrypt data
			for (DWORD i = CryptoSize; i < size; i += CryptoSize)
			{
				buffer[i - 2] ^= cheimagice[(i + 2) % 512];
				buffer[i - 1] ^= cheimagice[(i + 1) % 512];
				buffer[i] ^= cheimagice[i % 512];
				TotalEncryptedBytes += 3;
			}

			//criptare STATICA PHA!
			buffer[0x15] = 0x00; //Criptate v2


			printf("%s - > DEcrypted %d Bytes!\n", s.c_str(), TotalEncryptedBytes);
		}
		VMProtectEnd();

		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;
}



void encrypt_dnt(const string& s)
{


	string outputF = "encrypted_dnt\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		VMProtectBeginVirtualization("DNT Encrypt");
		if (buffer[0x00] == 0xDA)
		{
			printf("%s - > Already encrypted, skipping!\n", s.c_str());
			return;
		}

		//
		buffer[0x00] = 0xDA;

		DWORD CryptoSize = buffer[4]*10;
		
		//encrypt header
		for (DWORD j = 5; j < CryptoSize; j++)
		{
			buffer[j] ^= cheimagice[j % 512];
		}

		//encrypt data 32byte block!
		for (DWORD i = CryptoSize; i < size; i += CryptoSize)
		{
			for (DWORD j = 0; j < 32; j++)
			{
				buffer[i + j] ^= cheimagice[(j + 2) % 512];
			}
		}



		VMProtectEnd();

		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;
}


void decrypt_dnt(const string& s)
{



	string outputF = "decrypted_dnt\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		if (buffer[0x00] != 0xDA)
		{
			printf("%s - > Not encrypted, skipping!\n", s.c_str());
			return;
		}

		buffer[0] = 0x00;
 
		DWORD CryptoSize = buffer[4] * 10;


		VMProtectBeginVirtualization("DNT Decrypt");

		//encrypt header
		for (DWORD j = 5; j < CryptoSize; j++)
		{
			buffer[j] ^= cheimagice[j % 512];
		}

		//encrypt data
		for (DWORD i = CryptoSize; i < size; i += CryptoSize)
		{
			buffer[i - 2] ^= cheimagice[(i + 2) % 512];
			buffer[i - 1] ^= cheimagice[(i + 1) % 512];
			buffer[i] ^= cheimagice[i % 512];
		}

		VMProtectEnd();


		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;

	
}


void encryptTEST(const string& s)
{
	string outputF = "encryptedACT-DNT-XML\\" + s;
	std::ifstream infile(s, std::ifstream::binary);
	bool isAct = false;

	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];
		BYTE *outBuffer = new BYTE[size*2];
		infile.seekg(0, ios::beg);	
		infile.read((char*)buffer, size);

		DWORD forSize = size+5;
		for (DWORD i = 0; i < forSize; i++)
		{
			outBuffer[i] = 0x00;
		}

		if (has_suffix(outputF, ".act"))
		{
			isAct = true;
		}

		DWORD NewSize = compress(buffer, size, outBuffer);

		BYTE bFileSize[4] = { 0 };
		std::vector<BYTE> bytes = intToBytes(NewSize);
		for (int i = 0; i < bytes.size(); i++)
			bFileSize[i] = bytes[i];

		if (isAct){
			if (outBuffer[size - 1] == 0x00 && outBuffer[size - 2] == 0x00 && outBuffer[size - 3] == 0x00 && outBuffer[size - 4] == 0x00 && outBuffer[size - 5] == 0x00 && outBuffer[size - 6] == 0x00 && outBuffer[size - 7] == 0x00)
			{
				outBuffer[size - 1] = bFileSize[0];
				outBuffer[size - 2] = bFileSize[1];
				outBuffer[size - 3] = bFileSize[2];
				outBuffer[size - 4] = bFileSize[3];
				outBuffer[size - 5] = 0xDE;


				outBuffer[size - 7] ^= cheimagice[0];
				outBuffer[size - 6] ^= cheimagice[1];
				outBuffer[size - 4] ^= cheimagice[2];
				outBuffer[size - 3] ^= cheimagice[3];
				outBuffer[size - 2] ^= cheimagice[4];
				outBuffer[size - 1] ^= cheimagice[5];
			}
			else{
				NewSize = 0; //..
			}
		}

		//
		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		if (NewSize == 0)
		{
			printf("%s NOT CRYPTED!!!!\n", s.c_str());
		}
		else{
			printf("%s OK!\n", s.c_str());
		}

		if (NewSize == 0)
		{
			outfile.write((char*)buffer, size); //bagam un intreg pt original file size
		}
		else if(NewSize>0){
			//scrie header!
			/*if (has_suffix(outputF, ".dnt"))
			{
				//outBuffer[3] = 0xDD;
			}
			else if (has_suffix(outputF, ".act"))
			{
				//outBuffer[0] = 0xDD;
			}
			else if (has_suffix(outputF, ".xml"))
			{
				//outBuffer[size-7] = 0xDD;
			}*/
			if (!isAct)
			{
				outBuffer[size +4] = bFileSize[0];
				outBuffer[size +3] = bFileSize[1];
				outBuffer[size +2] = bFileSize[2];
				outBuffer[size +1] = bFileSize[3];
				outBuffer[size] = 0xDD;

				printf(" %.2X %.2X %.2X %.2X %.2X  \n", outBuffer[size], outBuffer[size + 1], outBuffer[size + 2], outBuffer[size + 3], outBuffer[size + 4]);

				//Criptam footer.
				outBuffer[size -2] ^= cheimagice[0];
				outBuffer[size -1] ^= cheimagice[1];
				outBuffer[size +1] ^= cheimagice[2];
				outBuffer[size +2] ^= cheimagice[3];
				outBuffer[size +3] ^= cheimagice[4];
				outBuffer[size +4] ^= cheimagice[5];
			}
			//criptam header
#if defined(RO) || defined(CHN)
			//decrypt header
			outBuffer[0] ^= cheimagice[10];
			outBuffer[1] ^= cheimagice[11];
			outBuffer[2] ^= cheimagice[12];
			outBuffer[3] ^= cheimagice[13];
#endif

			if (isAct)
			{
				outfile.write((char*)outBuffer, size); //bagam un intreg pt original file size

			}
			else
			{
				outfile.write((char*)outBuffer, size + 5); //bagam un intreg pt original file size
			}
				//outfile.write((char*)bFileSize, sizeof(bFileSize));  //original file size
		}

		outfile.close();


		infile.close();
		
		free(outBuffer);
		free(buffer);
	}

}

void decryptTEST(const string &s)
{
	string outputF = "decryptedACT-DNT-XML\\" + s;
	std::ifstream infile(s, std::ifstream::binary);
	bool isValid = TRUE;
	bool isAct = false;
	if (infile.is_open())
	{

		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];
		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		//decriptam footer.

		if (has_suffix(outputF, ".act"))
		{
			if (buffer[size - 5] != 0xDE)
			{
				isValid = FALSE;
			}
			else if (buffer[size - 5] == 0xDE)
			{
				buffer[size - 5] = 0x00;
			}
		}
		if (has_suffix(outputF, ".dnt") || has_suffix(outputF, ".xml"))
		{
			if (buffer[size-5] != 0xDD)
			{
				isValid = FALSE;
			}
			else if (buffer[size - 5] == 0xDD)
			{
				buffer[size - 5] = 0x00;
			}
		}

		if (isValid)
		{

#if defined(RO) || defined(CHN)
			//decrypt header
			buffer[0] ^= cheimagice[10];
			buffer[1] ^= cheimagice[11];
			buffer[2] ^= cheimagice[12];
			buffer[3] ^= cheimagice[13];
#endif
			//decrypt footer
			buffer[size - 7] ^= cheimagice[0];
			buffer[size - 6] ^= cheimagice[1];

			buffer[size - 4] ^= cheimagice[2];
			buffer[size - 3] ^= cheimagice[3];
			buffer[size - 2] ^= cheimagice[4];
			buffer[size - 1] ^= cheimagice[5];
			//printf(" %.2X %.2X %.2X %.2X %.2X %.2X  \n", buffer[size], buffer[size - 1], buffer[size - 2], buffer[size - 3], buffer[size - 4], buffer[size - 5]);
			/*buffer[size - 6] ^= cheimagice[0];
			buffer[size - 5] ^= cheimagice[1];
			buffer[size - 4] ^= cheimagice[2];
			buffer[size - 3] ^= cheimagice[3];
			buffer[size - 2] ^= cheimagice[4];
			buffer[size - 1] ^= cheimagice[5];*/
		}
		//printf("%x %x %x %x\n", buffer[size - 4], buffer[size - 3], buffer[size - 2], buffer[size-1]);
		DWORD OrigFileSize = (buffer[size - 1] << 24) | (buffer[size - 2] << 16) | (buffer[size - 3] << 8) | (buffer[size - 4]);

		//printf("Orig Size: %d\n",OrigFileSize);

		BYTE *outBuffer = new BYTE[size];
		/*
		BYTE OrigSize[4] = { 0 };
		std::vector<BYTE> bytes = intToBytes(size);
		for (int i = 0; i < bytes.size(); i++)
		OrigSize[i] = bytes[i];
		*/
		DWORD NewSize = 0;
		if (isValid)
			NewSize = decompress(buffer, OrigFileSize, outBuffer); //scoatem int din size

		if (isValid == FALSE)
		{
			printf("%s NOT CRYPTED!!!!\n", s.c_str());
		}
		else{
			printf("%s OK!\n", s.c_str());
		}
		
		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);
		if (isValid)
		{
			outfile.write((char*)outBuffer, NewSize);
		}else{
			outfile.write((char*)buffer, size);
		}

		outfile.close();

		infile.close();

		free(outBuffer);
		free(buffer);
		
	}
}