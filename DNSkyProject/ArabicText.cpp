#include <string>
#include <windows.h>
#include "ArabicText.h"
#include "ArabicShaping.h"


bool isArabic(WCHAR *c)
{
//	bool hasRandALCat = 0;
	DWORD size = wcslen(c);
	for (DWORD i = 0; i < size; i++)
	{
		if (c[i] == 0xff00)
		{
			return false;
		}

		if (c[i] >= 0x5BE && c[i] <= 0x10B7F)
		{
			return true;


			if (c[i] <= 0x85E)
			{
				if (c[i] == 0x5BE)                       return true;// 1;
				else if (c[i] == 0x5C0)                   return true;// 1;
				else if (c[i] == 0x5C3)                   return true;// 1;
				else if (c[i] == 0x5C6)                   return true;// 1;
				else if (0x5D0 <= c[i] && c[i] <= 0x5EA)     return true;// 1;
				else if (0x5F0 <= c[i] && c[i] <= 0x5F4)     return true;// 1;
				else if (c[i] == 0x608)                   return true;// 1;
				else if (c[i] == 0x60B)                   return true;// 1;
				else if (c[i] == 0x60D)                   return true;// 1;
				else if (c[i] == 0x61B)                   return true;// 1;
				else if (0x61E <= c[i] && c[i] <= 0x64A)     return true;// 1;
				else if (0x66D <= c[i] && c[i] <= 0x66F)     return true;// 1;
				else if (0x671 <= c[i] && c[i] <= 0x6D5)     return true;// 1;
				else if (0x6E5 <= c[i] && c[i] <= 0x6E6)     return true;// 1;
				else if (0x6EE <= c[i] && c[i] <= 0x6EF)     return true;// 1;
				else if (0x6FA <= c[i] && c[i] <= 0x70D)     return true;// 1;
				else if (c[i] == 0x710)                   return true;// 1;
				else if (0x712 <= c[i] && c[i] <= 0x72F)     return true;// 1;
				else if (0x74D <= c[i] && c[i] <= 0x7A5)     return true;// 1;
				else if (c[i] == 0x7B1)                   return true;// 1;
				else if (0x7C0 <= c[i] && c[i] <= 0x7EA)     return true;// 1;
				else if (0x7F4 <= c[i] && c[i] <= 0x7F5)     return true;// 1;
				else if (c[i] == 0x7FA)                   return true;// 1;
				else if (0x800 <= c[i] && c[i] <= 0x815)     return true;// 1;
				else if (c[i] == 0x81A)                   return true;// 1;
				else if (c[i] == 0x824)                   return true;// 1;
				else if (c[i] == 0x828)                   return true;// 1;
				else if (0x830 <= c[i] && c[i] <= 0x83E)     return true;// 1;
				else if (0x840 <= c[i] && c[i] <= 0x858)     return true;// 1;
				else if (c[i] == 0x85E)                   return true;// 1;
			}
			else if (c[i] == 0x200F)                      return true;// 1;
			else if (c[i] >= 0xFB1D)
			{
				if (c[i] == 0xFB1D)                       return true;// 1;
				else if (0xFB1F <= c[i] && c[i] <= 0xFB28)   return true;// 1;
				else if (0xFB2A <= c[i] && c[i] <= 0xFB36)   return true;// 1;
				else if (0xFB38 <= c[i] && c[i] <= 0xFB3C)   return true;// 1;
				else if (c[i] == 0xFB3E)                  return true;// 1;
				else if (0xFB40 <= c[i] && c[i] <= 0xFB41)   return true;// 1;
				else if (0xFB43 <= c[i] && c[i] <= 0xFB44)   return true;// 1;
				else if (0xFB46 <= c[i] && c[i] <= 0xFBC1)   return true;// 1;
				else if (0xFBD3 <= c[i] && c[i] <= 0xFD3D)   return true;// 1;
				else if (0xFD50 <= c[i] && c[i] <= 0xFD8F)   return true;// 1;
				else if (0xFD92 <= c[i] && c[i] <= 0xFDC7)   return true;// 1;
				else if (0xFDF0 <= c[i] && c[i] <= 0xFDFC)   return true;// 1;
				else if (0xFE70 <= c[i] && c[i] <= 0xFE74)   return true;// 1;
				else if (0xFE76 <= c[i] && c[i] <= 0xFEFC)   return true;// 1;
				else if (0x10800 <= c[i] && c[i] <= 0x10805) return true;// 1;
				else if (c[i] == 0x10808)                 return true;// 1;
				else if (0x1080A <= c[i] && c[i] <= 0x10835) return true;// 1;
				else if (0x10837 <= c[i] && c[i] <= 0x10838) return true;// 1;
				else if (c[i] == 0x1083C)                 return true;// 1;
				else if (0x1083F <= c[i] && c[i] <= 0x10855) return true;// 1;
				else if (0x10857 <= c[i] && c[i] <= 0x1085F) return true;// 1;
				else if (0x10900 <= c[i] && c[i] <= 0x1091B) return true;// 1;
				else if (0x10920 <= c[i] && c[i] <= 0x10939) return true;// 1;
				else if (c[i] == 0x1093F)                 return true;// 1;
				else if (c[i] == 0x10A00)                 return true;// 1;
				else if (0x10A10 <= c[i] && c[i] <= 0x10A13) return true;// 1;
				else if (0x10A15 <= c[i] && c[i] <= 0x10A17) return true;// 1;
				else if (0x10A19 <= c[i] && c[i] <= 0x10A33) return true;// 1;
				else if (0x10A40 <= c[i] && c[i] <= 0x10A47) return true;// 1;
				else if (0x10A50 <= c[i] && c[i] <= 0x10A58) return true;// 1;
				else if (0x10A60 <= c[i] && c[i] <= 0x10A7F) return true;// 1;
				else if (0x10B00 <= c[i] && c[i] <= 0x10B35) return true;// 1;
				else if (0x10B40 <= c[i] && c[i] <= 0x10B55) return true;// 1;
				else if (0x10B58 <= c[i] && c[i] <= 0x10B72) return true;// 1;
				else if (0x10B78 <= c[i] && c[i] <= 0x10B7F) return true;// 1;
			}
		}
	}

	return false;
}


std::wstring ReverseBackLetterandNumbers(std::wstring text)
{
	std::wstring StrNou = text;


	const wchar_t * newString = text.c_str();
	int size = wcslen(newString);
	//
	int start = 0;
	int end = 0;
	int alocat = 0;
	std::wstring temp;
	//
	for (int i = 0; i < size; i++)
	{		
		if ((WORD)newString[i] > 10 && (WORD)newString[i] < 255 && (WORD)newString[i] != 0x0020 && alocat == 0)
		{
			if (((WORD)newString[i] >= L'a' && (WORD)newString[i] <= L'z') || ((WORD)newString[i] >= L'A' && (WORD)newString[i] <= L'Z') || ((WORD)newString[i] >= L'0' && (WORD)newString[i] <= L'9'))
			{
				start = i;
				alocat = 1;
				//
			}
		}
		if ((WORD)newString[i] > 255 && alocat == 1 ||
			i == size-1 && alocat == 1)
		{
		//	char msg[255];
	//		sprintf(msg, "START: %d END : %d", start,end);
		//	MessageBoxA(NULL, msg, "OK", MB_OK);
			end = i;
			if ((end - start) > 1)
			{
				//if start = 0 sa fie fara +1.
				if (start == 0)
				{
					temp = StrNou.substr(start, (end - start) + 1);
				}
				else{
					temp = StrNou.substr(start, (end - start));
				}
				
				std::reverse(temp.begin(), temp.end());

				int k = start;
				int tempsize = temp.size();



				for (int j = 0; j < tempsize; j++)
				{
					StrNou[k] = temp[j];
					k++;
				}
				start = 0;
				end = 0;
				k = 0;
				alocat = 0;
			}
		}


	}


	//fix fucking reverse new line :)
	if (StrNou[0] == 0xA)
	{
		for (int i = 1; i < size; i++)
		{
			StrNou[i - 1] = StrNou[i];
		}
		StrNou[size-1] = 0xA;
	}


	if (wcsstr(StrNou.c_str(),L"/notice") != NULL)
	{
#ifdef DEBUG
		for (int i = 0; i < size; i++)
			printf("%.4X ", (WORD)StrNou[i]);
#endif
		std::string::size_type found = StrNou.find(L"/notice");
		StrNou.erase(found, 8);

		std::wstring noticeString = L"/notice ";
		int sizeNew = noticeString.size();
		int sizeStrNou = StrNou.size();
		
		//prima data rescirem textu mai sus!
		int j = 8;
		for (int i = 0; i < sizeStrNou; i++)
		{
			noticeString[j] = StrNou[i];
			j++;
		}
		j = 0;

		return noticeString;
	}

	return StrNou;
}


void ArabicTextTryFix(WCHAR * text)
{
	int size = wcslen(text);
	bool skip = false;
	for (int i = 0; i < size; i++)
	{
		if (text[i] == GetNameLinkSeperator())
		{
			if (text[i + 1] == 's')
			{
				skip = true;
			}
		}
	}

	if (isArabic(text) && skip == false){
		

		/*std::wstring try2 = s;

		uint32_t*	arabic_cp = new uint32_t[size];
		uint32_t	acp_size = get_presentation_form_b(size, (unsigned char*)text, size * sizeof(uint32_t), arabic_cp);

		for (size_t i = 0; i < acp_size; ++i)
			try2[i] = arabic_cp[i];
			*/

		std::wstring try2 = shape_arabic(text,size);
		std::reverse(try2.begin(), try2.end());

		std::wstring try3 = ReverseBackLetterandNumbers(try2);

		const wchar_t * test2 = try3.c_str();

		DWORD newSize = wcslen(test2);

		for (DWORD i = 0; i < newSize; i++)
		{
			text[i] = test2[i];
		}
		text[newSize+1] = 0x00; //its ok

		free((void*)test2);
	}

}