#ifndef _CCRC32_H
#define _CCRC32_H

#define CRC32_POLYNOMIAL 0x04C11DB7
#define CRC32BUFSZ 1024

class CCRC32
{
public:
	void Initialize(void);
	unsigned long FileCRC(const char *sFileName);
	unsigned long FullCRC(unsigned char *sData, unsigned long ulLength);
	void PartialCRC(unsigned long *ulInCRC, unsigned char *sData, unsigned long ulLength);

private:
	unsigned long Reflect(unsigned long ulReflect, char cChar);
	unsigned long ulTable[256];
};

#endif