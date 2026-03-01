#include "functions.h"

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"

#define MAX_BLOCK_SIZE 65536
#define INT_OFFSET 4

void _reallocMemory(mem_int_scan_result *misr) {
    misr->data = realloc(misr->data, sizeof(AddrValueElement) * misr->capacity);
    assert(misr->data);
    misr->capacity *= 2;
}

uint32_t extractValue(BYTE *bytes, int offset) {
    return bytes[offset] | (bytes[offset + 1] << 8) | (bytes[offset + 2] << 16) | (bytes[offset + 3] << 24);
}

unsigned __stdcall ThreadFunc(LPVOID arg) {
    threads_scan *ts = (threads_scan *) arg;
    MEMORY_BASIC_INFORMATION mbi;

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
                            if (ts->misr->len > ts->misr->capacity) {
                                printf("MT/func.h line 66 realloc\n");
                                _reallocMemory(ts->misr);
                            }

                            ts->misr->data[ts->misr->len].address = tmp_addr + offset;
                            ts->misr->data[ts->misr->len].value = ts->value;
                            ts->misr->len++;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}
