#include "ms.h"

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[94m"

void init(mem_int_scan_result **result1) {
    *result1 = ( mem_int_scan_result *) malloc(sizeof(mem_int_scan_result) * 100);
    (*result1)[0].len = 0;
    (*result1)[0].capacity = 100;
    if (result1 == NULL)
        printf(RED"Init Error. Memory Allocation Failed.\n"DT);
}

void scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed) {
    *isFirstScanUsed = 1;
    init(int_result);

    threads_scan *t1 = malloc(sizeof(threads_scan)), *t2 = malloc(sizeof(threads_scan));

    t1->hProcess = hProcess;
    t1->misr = *int_result;
    t1->value = value;
    t1->start = 0x0;
    t1->stop = 0x1FFFFFFFFFFF;

    t2->hProcess = hProcess;
    t2->misr = *int_result;
    t2->value = value;
    t2->start = 0x200000000000;
    t2->stop = 0x3FFFFFFFFFFF;

    unsigned hThreadId1, hThreadId2;
    HANDLE hThread1 = ( HANDLE ) _beginthreadex(NULL, 0, ThreadFunc, t1, 0, &hThreadId1);
    _errorcheckthread(hThread1);
    HANDLE hThread2 = ( HANDLE ) _beginthreadex(NULL, 0, ThreadFunc, t2, 0, &hThreadId2);
    _errorcheckthread(hThread2);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    if (CloseHandle(hThread1) == 0)
        printf(RED"CloseHandle 1 failed. Error: %d\n"DT, GetLastError());
    if (CloseHandle(hThread2) == 0)
        printf(RED"CloseHandle 2 failed. Error: %d\n"DT, GetLastError());

    if ((*int_result)[0].len == 0)
            printf(RED"No addresses with the value %lu found\n"DT, value);
     else {
        printf(GREEN"Found %d address/es with the value %lu\n"DT, (*int_result)[0].len, value);
        printf(BLUE"Address      | Value\n"DT);
        printf(BLUE"----------------------\n"DT);
        for (int i = 0; i < (*int_result)[0].len; ++i) {
            if ((*int_result)[i].address != 0)
                printf(BLUE"| %llx |  %lu\n"DT, (*int_result)[i].address, value);
        }
        printf(BLUE"----------------------\n"DT);
    }
}

void next_scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed) {
    if (*isFirstScanUsed == 0)
            printf(RED"You can't scan anything next without first scan.\n"DT);
    else {
        unsigned long tmp_value;
        uintptr_t last_addr;
        int counter = 0;
        if ((*int_result)[0].len != 0) {
            printf(BLUE"Address      | Value\n"DT);
            printf(BLUE"----------------------\n"DT);
            for (int i = 0; i < (*int_result)[0].len; ++i) {
                if (ReadProcessMemory(hProcess, (LPCVOID) (*int_result)[i].address, &tmp_value, sizeof(unsigned long), NULL) != 0) {
                    if (tmp_value == value) {
                        last_addr = (*int_result)[i].address;
                        printf(BLUE"| %llx |  %lu\n"DT, (*int_result)[i].address, tmp_value);
                    }
                }
            }
            printf(BLUE"----------------------\n"DT);
        }
        else
            printf(RED"No addresses with the value %lu found\n"DT, value);
    }
}