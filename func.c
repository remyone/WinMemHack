#include "functions.h"

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define THREAD_CREATE_ERROR -25
#define INT_OFFSET 4
#define MAX_BLOCK_SIZE 65536

int _errorcheckthread(HANDLE hThread) {
    if (hThread == NULL) {
        printf(RED"Failed to create thread [ID : %ld]"DT, GetCurrentThreadId());
        return THREAD_CREATE_ERROR;
    }

    return 0;
}

void _reallocMemory(mem_int_scan_result **misr) {
    if ((*misr)[0].len == (*misr)[0].capacity) {
        (*misr)[0].capacity *= 2;
        *misr = realloc(*misr, (*misr)[0].capacity * sizeof(mem_int_scan_result));
    }
}

void fillMainMisr(mem_int_scan_result *misr, mem_int_scan_result **misr2) {
    for (int i = 0; i < misr->len; ++i, (*misr2)[0].len++) {
        _reallocMemory(misr2);
        (*misr2)[(*misr2)[0].len].address = misr[i].address;
    }
}

uint32_t extractValue(BYTE *bytes, int offset) {
    return bytes[offset] | (bytes[offset + 1] << 8) | (bytes[offset + 2] << 16) | (bytes[offset + 3] << 24);
}

unsigned __stdcall ThreadFunc(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;

    mem_int_scan_result *misr2 = ( mem_int_scan_result *) malloc(sizeof(mem_int_scan_result) * 100);
    if (misr2 == NULL) {
        printf(RED"Memory allocation failed\n"DT);
        free(misr2);
        return 1;
    }
    else {
        misr2->len = 0;
        misr2->capacity = 100;
    }

    BYTE buffer[MAX_BLOCK_SIZE];
    SIZE_T bytesRead;
    uint32_t tmp_value;

    for (uintptr_t addr = ts->start; addr <= ts->stop; addr += mbi.RegionSize) {
        if (VirtualQueryEx(ts->hProcess, (LPCVOID) addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0) {
            if (GetLastError() != ERROR_SUCCESS) {
                printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
                addr += 0x1000;
                continue;
            }
        }

        if (mbi.RegionSize == 0)
            break;

        if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE || 
            mbi.Protect & PAGE_EXECUTE_READWRITE) && !(mbi.Protect & PAGE_GUARD)) {
            for (uintptr_t tmp_addr = addr; tmp_addr < (addr + mbi.RegionSize); tmp_addr += MAX_BLOCK_SIZE) {
            //вычисляем кол-во байт для чтения, чтобы не выйти за пределы региона и не было ошибок с чтением
                SIZE_T toRead = (SIZE_T) min(MAX_BLOCK_SIZE, (addr + mbi.RegionSize) - tmp_addr);
            //проверяем, если кол-во байт для чтения и кол-во прочитанных байт совпадают
                if (ReadProcessMemory(ts->hProcess, (LPCVOID) tmp_addr, buffer, toRead, &bytesRead) && bytesRead == toRead) {
                    for (int offset = 0; offset + sizeof(unsigned long) <= bytesRead; offset += INT_OFFSET) {
                        tmp_value = extractValue(buffer, offset);
                        if (tmp_value == ts->value) {
                            _reallocMemory(&misr2);
                            misr2[misr2->len].address = tmp_addr + offset;
                            misr2[misr2->len].value = ts->value;
                            misr2->len++;
                        }
                    }
                }
            }
        }
    }

    if (misr2->len != 0)
        fillMainMisr(misr2, &ts->misr);

    free(misr2);
    return 0;
}