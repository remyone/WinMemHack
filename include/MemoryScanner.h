#ifndef MY_MS_H
#define MY_MS_H

#include "MemoryThread.h"

extern int _errorcheckthread(HANDLE hThread);
extern unsigned __stdcall ThreadFunc(LPVOID arg);
extern void scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed);
extern void next_scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed);

#endif