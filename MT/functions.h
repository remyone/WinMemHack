#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <process.h>
#include <stdint.h>

typedef struct {
    uintptr_t address;
    int value;
    int len;
} mem_int_scan_result;

typedef struct {
    HANDLE hProcess;
    //MEMORY_BASIC_INFORMATION mbi;
    mem_int_scan_result *misr;
    unsigned long value;
} threads_scan;

extern int _errorcheckthread(HANDLE hThread);
extern unsigned __stdcall ThreadFunc1(LPVOID arg);
extern unsigned __stdcall ThreadFunc2(LPVOID arg);
extern unsigned __stdcall ThreadFunc3(LPVOID arg);
extern unsigned __stdcall ThreadFunc4(LPVOID arg);