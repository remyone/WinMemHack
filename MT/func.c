#include "functions.h"

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define THREAD_CREATE_ERROR -25

int _errorcheckthread(HANDLE hThread) {
    if (hThread == NULL) {
        printf(RED"Failed to create thread [ID : %ld]"DT, GetCurrentThreadId());
        return THREAD_CREATE_ERROR;
    }

    return 0;
}


unsigned __stdcall ThreadFunc1(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long tmp_value;

    for (uintptr_t addr = 0x0; addr <= 0x1ffffffffff; addr += mbi.RegionSize) {
        if (VirtualQueryEx(ts->hProcess, (LPCVOID) addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
            addr += mbi.RegionSize;
        }

        if (mbi.RegionSize == 0)   
            break;

        if (mbi.State != MEM_RESERVE && mbi.Protect != PAGE_NOACCESS) {
            for (uintptr_t tmp_addr = addr; tmp_addr <= (addr + mbi.RegionSize); tmp_addr += sizeof(unsigned long)) {
                if (ReadProcessMemory(ts->hProcess, (LPCVOID) tmp_addr, &tmp_value, sizeof(unsigned long), NULL) == 0)
                    continue;
                if (tmp_value == ts->value) {
                    printf(GREEN"Found %d at %llx\n"DT, ts->value, tmp_addr);
                    /*int_result[int_result->address].address = tmp_addr;
                    int_result->len++;*/
                }
            }
        }
    }
}

unsigned __stdcall ThreadFunc2(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long tmp_value;

    for (uintptr_t addr = 0x2000000000; addr <= 0x3ffffffffff; addr += mbi.RegionSize) {
        if (VirtualQueryEx(ts->hProcess, (LPCVOID) addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
            addr += mbi.RegionSize;
        }

        if (mbi.RegionSize == 0)   
            break;

        if (mbi.State != MEM_RESERVE && mbi.Protect != PAGE_NOACCESS) {
            for (uintptr_t tmp_addr = addr; tmp_addr <= (addr + mbi.RegionSize); tmp_addr += sizeof(unsigned long)) {
                if (ReadProcessMemory(ts->hProcess, (LPCVOID) tmp_addr, &tmp_value, sizeof(unsigned long), NULL) == 0)
                    continue;
                if (tmp_value == ts->value) {
                    printf(GREEN"Found %d at %llx\n"DT, ts->value, tmp_addr);
                    /*int_result[int_result->address].address = tmp_addr;
                    int_result->len++;*/
                }
            }
        }
    }
}

unsigned __stdcall ThreadFunc3(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long tmp_value;

    for (uintptr_t addr = 0x4000000000; addr <= 0x5ffffffffff; addr += mbi.RegionSize) {
        if (VirtualQueryEx(ts->hProcess, (LPCVOID) addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
            addr += mbi.RegionSize;
        }

        if (mbi.RegionSize == 0)   
            break;

        if (mbi.State != MEM_RESERVE && mbi.Protect != PAGE_NOACCESS) {
            for (uintptr_t tmp_addr = addr; tmp_addr <= (addr + mbi.RegionSize); tmp_addr += sizeof(unsigned long)) {
                if (ReadProcessMemory(ts->hProcess, (LPCVOID) tmp_addr, &tmp_value, sizeof(unsigned long), NULL) == 0)
                    continue;
                if (tmp_value == ts->value) {
                    printf(GREEN"Found %d at %llx\n"DT, ts->value, tmp_addr);
                    /*int_result[int_result->address].address = tmp_addr;
                    int_result->len++;*/
                }
            }
        }
    }
}

unsigned __stdcall ThreadFunc4(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long tmp_value;

    for (uintptr_t addr = 0x6000000000; addr <= 0x7ffffffffff; addr += mbi.RegionSize) {
        if (VirtualQueryEx(ts->hProcess, (LPCVOID) addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
            addr += mbi.RegionSize;
        }

        if (mbi.RegionSize == 0)   
            break;

        if (mbi.State != MEM_RESERVE && mbi.Protect != PAGE_NOACCESS) {
            for (uintptr_t tmp_addr = addr; tmp_addr <= (addr + mbi.RegionSize); tmp_addr += sizeof(unsigned long)) {
                if (ReadProcessMemory(ts->hProcess, (LPCVOID) tmp_addr, &tmp_value, sizeof(unsigned long), NULL) == 0)
                    continue;
                if (tmp_value == ts->value) {
                    printf(GREEN"Found %d at %llx\n"DT, ts->value, tmp_addr);
                    /*int_result[int_result->address].address = tmp_addr;
                    int_result->len++;*/
                }
            }
        }
    }
}