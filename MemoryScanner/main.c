#include "ms.h"

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[94m"
#define THREAD_CREATE_ERROR -25

int _errorcheckthread(HANDLE hThread) {
    if (hThread == NULL) {
        printf(RED"Failed to create thread [ID : %ld]"DT, GetCurrentThreadId());
        return THREAD_CREATE_ERROR;
    }

    return 0;
}

void init(mem_int_scan_result **result) {
   *result = (mem_int_scan_result *) malloc(sizeof(mem_int_scan_result));
    assert(*result);
   (*result)->data = (AddrValueElement *) malloc(sizeof(AddrValueElement) * 50);
    assert((*result)->data);
   (*result)->len = 0;
   (*result)->capacity = 50;
}

void destroy_misr(mem_int_scan_result **result) {
    free((*result)->data);
    free(*result);
    (*result)->len = 0;
    (*result)->capacity = 0;
    *result = NULL;
}

void scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result **int_result, bool *isFirstScanUsed) {
    *isFirstScanUsed = 1;
    init(int_result);

    threads_scan *t1 = (threads_scan *) malloc(sizeof(threads_scan)), *t2 = (threads_scan *) malloc(sizeof(threads_scan));

    t1->hProcess = hProcess;
    t1->misr = NULL;
    init(&(t1->misr));
    t1->value = value;
    t1->start = 0x0;
    t1->stop = 0x1FFFFFFFFFFF;

    HANDLE hThread1 = ( HANDLE ) _beginthreadex(NULL, 0, ThreadFunc, t1, 0, NULL);
    _errorcheckthread(hThread1);

    t2->hProcess = hProcess;
    t2->misr = NULL;
    init(&(t2->misr));
    t2->value = value;
    t2->start = 0x200000000000;
    t2->stop = 0x3FFFFFFFFFFF;
    HANDLE hThread2 = ( HANDLE ) _beginthreadex(NULL, 0, ThreadFunc, t2, 0, NULL);
    _errorcheckthread(hThread2);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    if (CloseHandle(hThread1) == 0)
        printf(RED"CloseHandle 1 failed. Error: %d\n"DT, GetLastError());
    if (CloseHandle(hThread2) == 0)
        printf(RED"CloseHandle 2 failed. Error: %d\n"DT, GetLastError());

    free(t1);
    free(t2);

    for (int i = 0; i < t1->misr->len; ++i) {
        if ((*int_result)->len > (*int_result)->capacity)
            _reallocMemory(*int_result);

        (*int_result)->data[i].address = t1->misr->data[i].address;
        (*int_result)->data[i].value = t1->misr->data[i].value;
        (*int_result)->len++;
    }

    for (int i = t1->misr->len; i < t2->misr->len; ++i) {
        if ((*int_result)->len > (*int_result)->capacity)
            _reallocMemory(*int_result);
            
        (*int_result)->data[i].address = t2->misr->data[i].address;
        (*int_result)->data[i].value = t2->misr->data[i].value;
        (*int_result)->len++;
    }

    if ((*int_result)->len == 0)
            printf(RED"No addresses with the value %lu found\n"DT, value);
     else {
        printf(GREEN"Found %d address/es with the value %lu\n"DT, (*int_result)->len, value);
        printf(BLUE"Address      | Value\n"DT);
        printf(BLUE"----------------------\n"DT);
        for (int i = 0; i < (*int_result)->len; ++i) {
            if ((*int_result)->data[i].address != 0)
                printf(BLUE"| 0x%llx |  %lu\n"DT, (*int_result)->data[i].address, (*int_result)->data[i].value);
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
                if (ReadProcessMemory(hProcess, (LPCVOID) (*int_result)->data[i].address, &tmp_value, sizeof(unsigned long), NULL) != 0) {
                    if (tmp_value == value) {
                        last_addr = (*int_result)->data[i].address;
                        printf(BLUE"| %llx |  %lu\n"DT, (*int_result)->data[i].address, tmp_value);
                    }
                }
            }
            printf(BLUE"----------------------\n"DT);
        }
        else
            printf(RED"No addresses with the value %lu found\n"DT, value);
    }
}
