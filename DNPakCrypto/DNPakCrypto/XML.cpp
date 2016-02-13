#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <dirent.h>
#include <conio.h>
#include <time.h>
#include "..\\..\\DNSkyProject\\vlizer.h"
#include "Header.h"
#include <random>
#include <stdio.h>
#include <VMProtectSDK.h>

using namespace std;

BYTE key[3] = { 0xAA, 0xCC, 0x73 };

void encrypt_xml(const string& s)
{

	string outputF = "encrypted_xml\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size+1];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		VMProtectBeginVirtualization("DNT Encrypt");
		if (buffer[size - 1] == 0xDA)
		{
			printf("%s - > Already encrypted, skipping!\n", s.c_str());
			return;
		}

		//encrypt header
		for (DWORD i = 0; i < size; i++)
		{
			buffer[i] ^=  key[i % 3];
		}

		printf("%s - > Encrypted!\n", s.c_str());
		//
		buffer[size] = 0xDA;


		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size+1);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;
}


void decrypt_xml(const string& s)
{

	string outputF = "decrypted_xml\\" + s;
	std::ifstream infile(s, std::ifstream::binary);


	if (infile.is_open())
	{
		infile.seekg(0, ios::end);
		int size = infile.tellg();
		BYTE *buffer = new BYTE[size+1];


		infile.seekg(0, ios::beg);
		infile.read((char*)buffer, size);

		if (buffer[size-1] != 0xDA)
		{
			printf("%s - > Not encrypted, skipping!\n", s.c_str());
			return;
		}


		VMProtectBeginVirtualization("DNT Decrypt");



		for (DWORD i = 0; i < size; i++)
		{
			buffer[i] ^= key[i % 3];
		}
		printf("%s - > Decrypted!\n", s.c_str());

	
		VMProtectEnd();


		ofstream outfile;
		outfile.open(outputF, ios::out | ios::binary);

		outfile.write((char*)buffer, size-1);
		outfile.close();


		infile.close();

		free(buffer);

	}
	return;


}