#ifndef _I_UTILITIES_H_
#define _I_UTILITIES_H_

/* Interface for the Utilities
 *
 * iUtilities.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

// general utilities
void logError(const wchar_t*);
void error(const wchar_t* msg, const wchar_t* more = 0);

// string utilities
unsigned strlen(const wchar_t* str);
int      strcmp(const wchar_t* a, const wchar_t* b);
wchar_t* strcat(wchar_t* dest, const wchar_t* src, int sizeDest);
wchar_t* strcpy(wchar_t* dest, const wchar_t* src, int sizeDest);
wchar_t* strcpyFromMB(wchar_t* dest, const char* src, int sizeDest);
char*    strcpyFromWC(char* dest, const wchar_t* src, int sizeDest);
wchar_t* nameWithDir(wchar_t*, const wchar_t*, const wchar_t*, int);
int      sprintf(wchar_t* str, int a, int b, int c);
int      sprintf(wchar_t* str, int a, const wchar_t* suffix);
const wchar_t* itowc(wchar_t* s, int a);

#endif