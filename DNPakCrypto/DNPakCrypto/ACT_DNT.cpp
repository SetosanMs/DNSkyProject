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
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_int_distribution<int> dist(0, chei.size());
				key[i] = chei[dist(mt)] & 0xFF; //hack
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
		
			printf("%s - > Encrypted %d Bytes, Potential Keys: %d , Total Keys: %d!\n", s.c_str(), TotalEncryptedBytes, MarimeChei, key_size);

	
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
			for (DWORD i = CryptoSize; i < size; i += CryptoSize)
			{
				buffer[i - 2] ^= cheimagice[(i + 2) % 512];
				buffer[i - 1] ^= cheimagice[(i + 1) % 512];
				buffer[i] ^= cheimagice[i % 512];
				TotalEncryptedBytes+=3;
			}

			//criptare STATICA PHA!
			buffer[0x15] = 0xDB; //Criptate v2


			printf("%s - > Encrypted %d Bytes! Version 2\n", s.c_str(), TotalEncryptedBytes);

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

		//encrypt data
		for (DWORD i = CryptoSize; i < size; i += CryptoSize)
		{
			buffer[i - 2] ^=  cheimagice[(i + 2) % 512];
			buffer[i - 1] ^=  cheimagice[(i + 1) % 512];
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
	string outputF = "encrypted_TEST\\" + s;
	std::ifstream infile(s, std::ifstream::binary);

	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size];
		BYTE *outBuffer = new BYTE[size];
		infile.seekg(0, ios::beg);	
		infile.read((char*)buffer, size);
		
		DWORD NewSize = compress(buffer, size, outBuffer);



		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)outBuffer, NewSize);
		outfile.close();


		infile.close();

		free(buffer);
	}
}