#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <process.h>
#include <stdint.h>

typedef struct {
    uintptr_t address;
    int value;
    int len;
    int capacity;
} mem_int_scan_result;

typedef struct {
    HANDLE hProcess;
    mem_int_scan_result *misr;
    uintptr_t start, stop;
    unsigned long value;
} threads_scan;

extern int _errorcheckthread(HANDLE hThread);
extern void _reallocMemory(mem_int_scan_result **misr);
extern void fillMainMisr(mem_int_scan_result *misr, mem_int_scan_result **misr2);
extern unsigned __stdcall ThreadFunc(LPVOID arg);

#endif