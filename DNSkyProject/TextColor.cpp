#include <Windows.h>
#include <string>
#include "ArabicText.h"
#include "TextColor.h"

void Colorify(wchar_t* msg)
{
	wchar_t *color = ColoredText(msg, 0, 197, 205,255);
	int size = wcslen(color);
	int i;
	for (i= 0; i < size; i++)
	{
		msg[i] = color[i];
	}
	msg[i] = '\0';
}

wchar_t* ColoredText(wchar_t* message, byte red, byte green, byte blue, byte alpha = 255)
{
	wchar_t temp[255];
	wsprintfW(temp, L"%c%s%c%s%.2X%.2X%.2X%.2X%s%c%s%c%s%s",
		GetNameLinkSeperator(), L"s", GetNameLinkSeperator(), L"c",
		alpha,red,green,blue,
		message,
		GetNameLinkSeperator(), L"e", GetNameLinkSeperator(), L"r",
		L'\0'
		);



	/*
	temp[0] = GetNameLinkSeperator();
	temp[1] = L's';
	temp[2] = GetNameLinkSeperator();
	temp[3] = L'c';
	temp[4] = alpha;
	temp[5] = red;
	temp[6] = green;
	temp[7] = blue;
	int size = wcslen(message);
	for (int i = 0; i < size; i++)
	{
		temp[i + 8] = message[i];
	}	
//	size += 7;
	temp[size+8] = GetNameLinkSeperator();
	temp[size+9] = L'e';
	temp[size+10] = GetNameLinkSeperator();
	temp[size+11] = L'r';
	*/
//	wsprintfW(temp, L"%x%s%x%s%x%x%x%x%s%x%s%x%s", GetNameLinkSeperator(), L"s", GetNameLinkSeperator(), L"c", alpha, red, green, blue, message, GetNameLinkSeperator(), L"e", GetNameLinkSeperator(),L"r");
/*	"\uff00s\uff00c{0:x2}{1:x2}{2:x2}{3:x2}{4}\uff00e\uff00r" */
	
	return temp;
}