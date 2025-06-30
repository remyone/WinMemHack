#ifndef MY_MS_H
#define MY_MS_H

#include "functions.h"
#include <stdbool.h>

extern void init(mem_int_scan_result **result1);
extern void scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed);
extern void next_scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed);

#endif