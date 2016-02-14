#include <Windows.h>
#include <stdio.h>
#include <lzo/lzoconf.h>
#include <lzo/lzo1x.h>
#include "lzo.h"

static const char *progname = NULL;
#define WANT_LZO_MALLOC 1
#define WANT_XMALLOC 1

#include "portab.h"


#pragma comment(lib,"lzo2.lib")

DWORD compress(BYTE* data,DWORD size,BYTE* out)
{

	int r;
	lzo_bytep in;
	lzo_bytep out;
	lzo_voidp wrkmem;
	lzo_uint in_len;
	lzo_uint out_len;
	lzo_uint new_len;


	if (lzo_init() != LZO_E_OK)
	{
		printf("internal error - lzo_init() failed !!!\n");
		printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
		return 4;
	}

	//initializam memoria de lucru lol...
	wrkmem = (lzo_voidp)xmalloc(LZO1X_1_MEM_COMPRESS);


	//folosind lzo1x_1 compress algorithm
	r = lzo1x_1_compress(data, size, out, &out_len, wrkmem);
	if (r == LZO_E_OK)
		printf("compressed %lu bytes into %lu bytes\n",
		(unsigned long)in_len, (unsigned long)out_len);
	else
	{
		/* this should NEVER happen */
		printf("internal error - compression failed: %d\n", r);
		return 2;
	}
	/* check for an incompressible block */
	if (out_len >= in_len)
	{
		printf("This block contains incompressible data.\n");
		return 0;
	}

	lzo_free(wrkmem);

	return out_len;
}


DWORD decompress(BYTE* data, DWORD size)
{

}