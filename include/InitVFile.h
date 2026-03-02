#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <process.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    uintptr_t address;
    int value;
} AddrValueElement;

typedef struct {
    AddrValueElement *data;
    int len;
    int capacity;
} mem_int_scan_result;

typedef struct {
    HANDLE hProcess;
    mem_int_scan_result *misr;
    uintptr_t start, stop;
    unsigned long value;
} threads_scan;

extern void init(mem_int_scan_result **result);
extern void destroy_misr(mem_int_scan_result **result);