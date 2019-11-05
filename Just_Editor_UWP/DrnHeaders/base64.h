#pragma once

#include "universal.h"
#include "Drn_Tools.h"

const uchar base64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

uchar* base64_decrypt(const uchar* cipher)
{
    size_t sLen = strlen((const char*)cipher), j = 0;
    while(cipher[--sLen] == '=');
    schar offset = 0;
    uchar* result = new uchar[sLen * 3 / 4], sIndex;
    result[0] = 0;
    for (size_t i = 0; i <= sLen; i++)
    {            
        sIndex = tellmewhere (base64_map, cipher[i], 64);
        result[j] += offset < 2 ? sIndex << (2 - offset) : sIndex >> (offset - 2);
        offset += 6;
        if (offset > 8)
        {
            offset -= 8;
            result[++j] = 0;
            result[j] = sIndex << (8 - offset);
        }
    }
    result[j + 1] = 0;
    
    return result;
}
 
uchar* base64_encrypt(const uchar* source)
{
    const size_t sLen = strlen((const char*)source);
    size_t j = 2 - (sLen - 1) % 3, i;
	if (j)
		j++;
    const size_t cLen = sLen * 4 / 3 + j;
	uchar* result = new uchar[cLen], offset = 0, b = 2;
    while(j)
        result[cLen - --j] = '=';
    
    result[0] = 0;
    for (i = 0; i < sLen; i++, j++, b += 2)
    {
        result[j] = base64_map[offset + (source[i] >> b)];
        offset = source[i] << (8 - b);
        offset >>= 2;
        if (i + 1 == sLen || b == 6)
        {
            result[++j] = base64_map[offset];
            b = 0;
            offset = 0;
        }
    }
    result[cLen] = 0;
    
    return result;
}

wchar_t* base64_decryptw(const wchar_t* wcipher)
{
	uchar* content = base64_decrypt((const uchar*)Drn_Tools::wctos(wcipher).c_str());
	wchar_t* result = Drn_Tools::ctowc((const char*)content);
//	free(content);

	return result;
}

wchar_t* base64_encryptw(const wchar_t* content)
{
	uchar* cipher = base64_encrypt((const uchar*)Drn_Tools::wctos(content).c_str());
	wchar_t* result = Drn_Tools::ctowc((const char*)cipher);
//	free(cipher);

	return result;
}