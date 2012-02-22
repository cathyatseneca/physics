/* Utilities Implementation
 *
 * iUtilities.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iAPIWindow.h" // for the Window Interface
#include "iUtilities.h" // for the Utility Interface

const wchar_t* itowc(wchar_t* s, int a);

//---------------------------------- Utilities --------------------------------
//
// The Utilities module supports the Framework
//
// strlen returns the length of str
//
unsigned strlen(const wchar_t* str) {

    unsigned len = 0u;
    while (*str++) len++;
	return len;
}

// strcmp compares string a and string b
//
int strcmp(const wchar_t* a, const wchar_t* b) { 

	int diff = 0;
    while (*a && *b && !(diff = *a++ - *b++));
    return diff;
}

// strcat catenates src to dest[sizeDest+1] without overflow and
// returns the address of dest[]
// this function is a safe version of strcat
//
wchar_t* strcat(wchar_t* dest, const wchar_t* str, int sizeDest) {

    wchar_t* rc = dest;
    *(dest + sizeDest) = '\0';
    while (*dest++) sizeDest--;
    dest--;
    while (sizeDest-- && (*dest++ = *str++)) ;

    return rc;
}

// strcpy copies src into dest[sizeDest+1] without overflow and
// returns the address of dest[]
// this function is a safe version of strcpy and a version of
// strncpy that does not require last character padding with a null
// byte
//
wchar_t* strcpy(wchar_t* dest, const wchar_t* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = *src++)) ;

    return dest;
}

// strcpyFromMB copies multi-byte src into wide character dest[sizeDest+1] 
// without overflow and returns the address of dest[]
//
wchar_t* strcpyFromMB(wchar_t* dest, const char* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = *src++)) ;

    return dest;
}

// strcpyFromWC copies wide character src into multi-byte dest[sizeDest+1] 
// without overflow and returns the address of dest[]
//
char* strcpyFromWC(char* dest, const wchar_t* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = char(*src++))) ;

    return dest;
}

// sprintf prints a, b, c into str
//
int sprintf(wchar_t* str, int a, int b, int c) {

    wchar_t s[11];
    
    strcpy(str, itowc(s, a), 10);
    strcat(str, L", ", 12);
    strcat(str, itowc(s, b), 22);
    strcat(str, L", ", 24);
    strcat(str, itowc(s, c), 34);

	return 0;
}

// sprintf prints integer a and a suffix string into str
//
int sprintf(wchar_t* str, int a, const wchar_t* suffix) {

    wchar_t s[11];

    strcpy(str, L" ", 1);
    strcat(str, itowc(s, a), 11);
    strcat(str, suffix, strlen(suffix) + 11);

    return 0;
}

// nameWithDir adds directory to filename to create nameWithDir
//
wchar_t* nameWithDir(wchar_t* nameWithDir, const wchar_t* directory, 
 const wchar_t* filename, int sizeDest) {

	strcpy(nameWithDir, directory, sizeDest);
	strcat(nameWithDir, L"\\", sizeDest);
	strcat(nameWithDir, filename, sizeDest);

	return nameWithDir;
}

// itowc converts int value into a wide character string
//
const wchar_t* itowc(wchar_t* s, int value) {

    wchar_t  r[12];
    wchar_t* rp = r;
    wchar_t* ss = s;

    if (!value)
        *ss++ = L'0';
    else {
        if (value < 0) {
            value = -value;
            *ss++ = L'-';
        }
        while (value) {
            *rp++ = value % 10 + L'0';
            value /= 10;
        }
        rp--;
        while (rp != r - 1)
            *ss++ = *rp--;
    }
    *ss = L'\0';

    return s;
}
 

