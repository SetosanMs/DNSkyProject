#include <Windows.h>
#include <vector>
#include "se.h"
#include "Crypto.h"
#include "Extras.h"
#include "Misc.h"
#include "PackingSystem.h"
#include "vlizer.h"
#include "lzo.h"


PackingSystem Packing;
PackFile FileCrypt;

PackingSystem::PackingSystem()
{

}


PackingSystem::~PackingSystem()
{
//they call me the cunt destroyer.
}


bool PackingSystem::AddFile(HANDLE handle,PackVersion version)
{
	//verificam sa adaug doar ralukat...
	if (version == PackVersion::NOTPACKED)
		return FALSE;

	if (LoadNewUI == FALSE && version == PackVersion::RALUKAT_v12)
		return FALSE;

	if (!this->GetHandle(handle))
	{
		data.push_back(new PackData(handle, version));
#ifdef DEBUG_PACKING
		printf("[PackingSystem] Added %d handle with version %d\n",handle,version);
#endif
		return TRUE;
	}

	return FALSE;
}

bool PackingSystem::GetHandle(HANDLE handle)
{
	for (std::vector<PackData*>::iterator it = data.begin(); it != data.end(); ++it) {
		if ((*it)->GetHandle() == handle)
		{
			return TRUE;
		}
	}

	return FALSE;
}

PackVersion PackingSystem::GetVersionByHandle(HANDLE handle)
{
	for (std::vector<PackData*>::iterator it = data.begin(); it != data.end(); ++it) {
		if ((*it)->GetHandle() == handle)
		{
			return (*it)->GetVersion();
		}
	}

	return PackVersion::NOTPACKED; //poate ca nu e bine dar meh. j/k e bun.
}

PackVersion PackingSystem::CalculateVersion(const char * version)
{
	if (strcmp(version, Stampv1) == 0)
	{
		return PackVersion::RALUKAT_v10;
	}
	if (strcmp(version, Stampv2) == 0)
	{
		return PackVersion::RALUKAT_v11;
	}	
	if (strcmp(version, Stampv3) == 0)
	{
		return PackVersion::RALUKAT_v12;
	}

	return PackVersion::NOTPACKED;
}


bool PackingSystem::isHeader(DWORD size)
{
	if (size == 0x110)
		return TRUE;

	return FALSE;
}

bool PackingSystem::isFooter(DWORD size)
{
	if (std::find(dwFooterReadSize.begin(), dwFooterReadSize.end(), size) != dwFooterReadSize.end())
		return TRUE;

	return FALSE;
}

void PackingSystem::DecryptHeader(BYTE * data) //static decryption , nu e nevoie de size.
{
	//VIRTUALIZER_DOLPHIN_WHITE_START
	for (DWORD i = 0x110; i != 0x20 - 1; i += -1)
	{
		((BYTE*)data)[i] ^= ((BYTE*)data)[i - 1] ^ cheimagice[i % 512]; 
	}
	//VIRTUALIZER_DOLPHIN_WHITE_END
	//Calculate FOOTER
	SPackingFileHeader *header = (SPackingFileHeader *)data;
	dwFooterReadSize.push_back(header->nFileCount*sizeof(SPackingFileInfo));
}

void PackingSystem::DecryptFooter(BYTE * data, DWORD size)
{
	
	for (DWORD i = size - 1; i != 0x32; i += -1)
	{
		((BYTE*)data)[i] ^= ((BYTE*)data)[i - 1] ^ cheimagice[i % 512];
	}

	this->DecryptFooter32Bytes(data);
}

void PackingSystem::DecryptFooter32Bytes(BYTE *data)
{
	for (DWORD i = 0; i < 0x32; i++)
	{
		((BYTE*)data)[i] ^= cheimagice[(i + 0x69) % 512];
	}
}

void PackingSystem::DecryptData(BYTE* data, DWORD size)
{
	
	for (DWORD i = 1; i < size; i++)
	{
		((BYTE*)data)[i] ^= cheimagice[i % 512];
	}
	
#ifdef DEBUG_PACKING
	printf("[PackingSystem] Decrypting Data from handle\n");
#endif
}

//---------------------------------------------------------------------
///added 07.02.2016 alin1337
FileVersion PackFile::CheckFileType(char* filename, BYTE* data, DWORD size)
{
	if (has_suffix(filename, ".act"))
	{
		if (data[0] == 0xDD)
		{
			((BYTE*)data)[0] = 0x00;
			decrypt_LZO(data, size);
			return RALUKAT_ACT;
		}
	}
	
	if (has_suffix(filename, ".dnt"))
	{
		if (data[3] == 0xDD)
		{
			((BYTE*)data)[3] = 0x00;
			decrypt_LZO(data, size);
			return RALUKAT_DNT;
		}
	}

	if (has_suffix(filename, ".xml"))
	{
		if (data[size-7] == 0xDD)
		{
			((BYTE*)data)[size-7] = 0x00;
			decrypt_LZO(data, size);
			return RALUKAT_XML;
		}
	}
	

	return ANOTHERFILE;
}

void PackFile::isPacked(FileVersion version,  BYTE* data, DWORD size)
{
#ifdef DEPRECATED
	if (version == RALUKAT_ACT)
	{
		if (data[0x15] == 0xDA)
		{
			decrypt_ACT_DA(data, size);
			return;
		}
		else if (data[0x15] == 0xDB)
		{
			decrypt_ACT_DB(data, size);
			return;
		}
	}

	if (version == RALUKAT_DNT)
	{
		if (data[0x00] == 0xDA)
		{
			decrypt_DNT(data, size);
			return;
		}
	}

	if (version == RALUKAT_XML)
	{
		if (data[size - 1] == 0xDA)
		{
			decrypt_XML(data, size);
			return;
		}
	}
#endif
}

//DNT
void PackFile::decrypt_DNT(BYTE* data, DWORD size)
{
#ifdef DEBUG
	printf("Decrypt DNT\n");
#endif
	((BYTE*)data)[0] = 0x00;

	DWORD CryptoSize = ((BYTE*)data)[4] * 10;

	//decrypt header
	for (DWORD j = 5; j < CryptoSize; j++)
	{
		((BYTE*)data)[j] ^= cheimagice[j % 512];
	}

	//decrypt data
	for (DWORD i = CryptoSize; i < size; i += CryptoSize)
	{
		((BYTE*)data)[i - 2] ^= cheimagice[(i + 2) % 512];
		((BYTE*)data)[i - 1] ^= cheimagice[(i + 1) % 512];
		((BYTE*)data)[i] ^= cheimagice[i % 512];
	}
}

//ACT Versiunea A
void PackFile::decrypt_ACT_DA(BYTE* data, DWORD size)
{
#ifdef DEBUG
	printf("Decrypt ACT A\n");
#endif
	//decrypt key length
	((BYTE*)data)[0x16] ^= ((BYTE*)data)[0x24];

	if (data[0x16] < 10) //fix
	{
		//get keys
		BYTE key_size = ((BYTE*)data)[0x16];
		BYTE key[9] = { 0 };

		for (BYTE i = 0; i < key_size; i++)
		{
			key[i] = ((BYTE*)data)[i + 0x17];
		}

		///start!
		DWORD i = 0x20;
		DWORD j = 0;

		for (; i < size; i++)
		{
			if (((BYTE*)data)[i] != 0x00)
			{
				((BYTE*)data)[i] ^= key[i % key_size];
			}
		}
	}
}

//ACT Versiunea B
void PackFile::decrypt_ACT_DB(BYTE* data, DWORD size)
{
#ifdef DEBUG
	printf("Decrypt ACT B\n");
#endif
	DWORD CryptoSize = ((BYTE*)data)[0x24] * 10;

	//encrypt header
	for (DWORD j = 0x28; j < CryptoSize; j++)
	{
		((BYTE*)data)[j] ^= cheimagice[j % 512];
	}

	//encrypt data
	for (DWORD i = CryptoSize; i < size; i += CryptoSize)
	{
		((BYTE*)data)[i - 2] ^= cheimagice[(i + 2) % 512];
		((BYTE*)data)[i - 1] ^= cheimagice[(i + 1) % 512];
		((BYTE*)data)[i] ^= cheimagice[i % 512];
	}
}


BYTE key_XML[3] = { 0xAA, 0xCC, 0x73 };
//XML 
void PackFile::decrypt_XML(BYTE* data, DWORD size)
{
#ifdef DEBUG
	printf("Decrypt XML\n");
#endif
	for (DWORD i = 0; i < size; i++)
	{
		((BYTE*)data)[i] ^= key_XML[i % 3];
	}

	((BYTE*)data)[size - 1] = 0x00;
}



////LZO algorithm 19.02.2016
bool decrypt_LZO(BYTE* data, DWORD size)
{
	data[size - 6] ^= cheimagice[0];
	data[size - 5] ^= cheimagice[1];
	data[size - 4] ^= cheimagice[2];
	data[size - 3] ^= cheimagice[3];
	data[size - 2] ^= cheimagice[4];
	data[size - 1] ^= cheimagice[5];

	DWORD OrigFileSize = (data[size - 4] << 24) | (data[size - 3] << 16) | (data[size - 2] << 8) | (data[size - 1]);

	BYTE* cData = new BYTE[OrigFileSize];
	memcpy(cData, data, OrigFileSize);
	decompress(cData, OrigFileSize, data);
	free(cData);
	return true;


	//nu trebuie sa se intample niciodata!!
#ifdef DEBUG
	printf("HIT FALSE DECRYPT!!\n");
#endif
	return false;
}
