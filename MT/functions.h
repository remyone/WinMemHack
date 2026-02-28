#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include "InitVFILE.h"

extern int _errorcheckthread(HANDLE hThread);
extern void _reallocMemory(mem_int_scan_result *misr);
unsigned __stdcall ThreadFunc(LPVOID arg);

#endif