#ifndef PACKINGSYSTEM_H
#define PACKINGSYSTEM_H
#pragma once

enum PackVersion
{
	RALUKAT_v10,
	RALUKAT_v11,
	RALUKAT_v12,
	RALUKAT_v13,
	RALUKAT_v14,

	NOTPACKED = 99
};

enum FileVersion
{
	RALUKAT_ACT,
	RALUKAT_DNT,
	RALUKAT_XML,

	ANOTHERFILE = 99
};

class PackFile
{
public:
	void CheckFileType(char* filename, BYTE* data, DWORD size);
	void isPacked(FileVersion version, BYTE* data, DWORD size);
	void decrypt_ACT_DA(BYTE* data, DWORD size);
	void decrypt_ACT_DB(BYTE* data, DWORD size);
	void decrypt_DNT(BYTE* data, DWORD size);
	void decrypt_XML(BYTE* data, DWORD size);
};
extern PackFile FileCrypt;

class PackData
{
	HANDLE handle;
	PackVersion version;
public:
	PackData(HANDLE handle, PackVersion ver){ this->handle = handle; this->version = ver; };
	~PackData(){ delete handle; };
	HANDLE GetHandle(){ return handle; }
	PackVersion GetVersion(){ return version; };
};


class PackingSystem
{
private:
	std::vector<PackData*> data;
	std::vector<DWORD> dwFooterReadSize;

	const char * Stampv1 = enc("Ralukat Labs Packing System 1.0");
	const char * Stampv2 = enc("Ralukat Labs Packing System 1.1");
	const char * Stampv3 = enc("Ralukat Labs Packing System 1.2");


public:
	PackingSystem();
	~PackingSystem();
	bool AddFile(HANDLE handle, PackVersion version);
	bool GetHandle(HANDLE handle);
	PackVersion GetVersionByHandle(HANDLE handle);
	PackVersion CalculateVersion(const char * version);
	void Check(HANDLE handle);
	void DecryptHeader(BYTE * data);
	void DecryptFooter(BYTE * data, DWORD size);
	void DecryptFooter32Bytes(BYTE *data);
	void DecryptData(BYTE * data, DWORD size);
	bool isHeader(DWORD size);
	bool isFooter(DWORD size);
};
extern PackingSystem Packing;
#endif