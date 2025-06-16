#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string.h>
#include <memoryapi.h>
#include <winnt.h>

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[94m"

enum value_type {
    BYTES_4
};

typedef struct {
    uintptr_t start;
    uintptr_t stop;
    int value_type;
} scan_settings;

typedef struct {
    uintptr_t address;
    int value;
    int len;
} mem_int_scan_result;

enum value_type val_type = BYTES_4;
scan_settings sc_settings = {0x0, 0x7fffffffffff, BYTES_4};

void print_scan_settings() {
    printf(BLUE"------------------------\n"DT);

    printf("Start : [%llx]\nStop : [%llx]\n", sc_settings.start, sc_settings.stop);
    if (sc_settings.value_type == BYTES_4)
        printf("Value Type : [4 Bytes]\n");
    else 
        printf(RED"Value type : [???]\n"DT);

    printf(BLUE"------------------------\n"DT);
}

DWORD _findProcessPidByName(char name[]) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf(RED"Snapshot error %d. Couldn't create a snapshot\n"DT, GetLastError());
        exit(1);
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, name) == 0) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);

    return -1;
}

void PrintProcessName(HANDLE hProcess) {
    char processName[MAX_PATH];
    if (GetModuleFileNameExA(hProcess, NULL, processName, MAX_PATH) == 0)
        printf(RED"Error %d: Failed to get process name\n"DT, GetLastError());
    else {
        printf(BLUE"------------------------------------------------------------------------------\n"DT);
        printf(BLUE"| Process name: %s |\n"DT, processName);
        printf(BLUE"------------------------------------------------------------------------------\n"DT);
    }
}

void scan_memory(HANDLE hProcess, unsigned long value, mem_int_scan_result *int_result) {
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long tmp_value;
    printf("Scanning....\n");
    for (uintptr_t addr = sc_settings.start; addr <= sc_settings.stop; addr += mbi.RegionSize) {
        if (VirtualQueryEx(hProcess, (LPCVOID) addr, &mbi, sizeof(mbi)) == 0)
            printf(RED"VirtualQueryEx failed. Error: %d\n"DT, GetLastError());
        for (uintptr_t tmp_addr = addr; tmp_addr <= (addr + mbi.RegionSize); tmp_addr += 4) {
            ReadProcessMemory(hProcess, (LPCVOID) tmp_addr, &tmp_value, sizeof(int), NULL);
            printf("Addr: [%llx]. Value: [%lu]\n", tmp_addr, tmp_value);
            if (tmp_value == value) {
                printf(GREEN"Found %d at %llx\n"DT, tmp_value, tmp_addr);
                int_result[int_result->len].address = tmp_addr;
                ++int_result->len;
            }
        }
    }
}

void init(mem_int_scan_result *result1) {
    result1 = calloc(100, sizeof(int) + sizeof(uintptr_t));
    result1->len = 0;
    if (result1 == NULL)
        printf(RED"Init error: memory allocation 1 failed.\n"DT);
}

void parse_commands(char buffer[1024], HANDLE hProcess, mem_int_scan_result *int_result) {
    int i = 0;
    char *token; 
    char *tokens[25];

    token = strtok(buffer, " |\\/+-*:[]{}");
    tokens[i] = token;

    while (token != NULL) {
        i++;
        token = strtok(NULL, " |\\/+-*:[]{}");
        tokens[i] = token;
    }

    if (strcmp(tokens[0], "exit") == 0)
        exit(0);
    else if (strcmp(tokens[0], "set") == 0) {
        if (i < 3)
            printf(RED"Usage: set address new_value\n"DT);
        else {
            uintptr_t address = strtoull(tokens[1], NULL, 16);
            int old_value;
            int new_value = atoi(tokens[2]);

            if(ReadProcessMemory(hProcess, (LPCVOID) address, &old_value, sizeof(int), NULL) == 0)
                printf(RED"Read failed. Error: %d\n"DT, GetLastError());

            if (old_value == new_value)
                printf(RED"old_value equals new_value. You can't change it!\n"DT, old_value, new_value);
            else {
                if (WriteProcessMemory(hProcess, (LPVOID) address, &new_value, sizeof(new_value), NULL))
                    printf(GREEN"Value updated! [%d -> %d]\n"DT, old_value, new_value);
                else
                    printf(RED"Write failed. Error: %d\n"DT, GetLastError());
            }
        }
    } else if (strcmp(tokens[0], "clear") == 0)
        system("cls");
    else if (strcmp(tokens[0], "first_scan") == 0) {
        unsigned long value = strtoul(tokens[1], NULL, 10);
        scan_memory(hProcess, value, int_result);

        if (int_result->len == 0)
            printf(RED"No addresses with the value %lu found\n"DT, value);
        else
            printf(GREEN"Found %d with the value %lu\n"DT, int_result->len, value);
            printf(BLUE"Address      | Value\n"DT);
            printf(BLUE"----------------------\n"DT);
            for (int i = 0; i < int_result->len; ++i)
                printf(BLUE"| %llx |  %d\n"DT, int_result[i].address, value);
            printf(BLUE"----------------------\n"DT);
    }
    else if (strcmp(tokens[0], "scan_settings") == 0)
        print_scan_settings();
    else if (strcmp(tokens[0], "edit") == 0 && strcmp(tokens[1], "scan_settings") == 0) {
        if (strcmp(tokens[2], "start") == 0)
            sc_settings.start = strtoull(tokens[3], NULL, 16);
        else if (strcmp(tokens[2], "stop") == 0)
            sc_settings.stop = strtoull(tokens[3], NULL, 16);
        else if (strcmp(tokens[2], "value_type") == 0)
            sc_settings.value_type = atoi(tokens[3]);
        
        printf(GREEN"Updated scan settings:\n"DT);
        print_scan_settings();
    }
        
    i = 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf(RED"Usage: ./main ProcessName.exe\n"DT);
        exit(1);
    }
 
    printf(GREEN" ----------------------------\n"DT);  
    printf(GREEN"| MEMORY EDITOR v1.0         |\n"DT);  
    printf(GREEN" ----------------------------\n"DT); 

    DWORD PID = _findProcessPidByName(argv[1]);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
                                  FALSE,
                                  PID);


    if (hProcess == NULL) {
        printf(RED"HANDLE ERROR %d\n"DT, GetLastError());
        exit(1);
    }

    mem_int_scan_result int_result[100];
    char buffer[1024];

    init(int_result);

    PrintProcessName(hProcess);

    printf(GREEN"Process has been opened successfully!\n"DT);

    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        parse_commands(buffer, hProcess, int_result);
    }

    CloseHandle(hProcess);
    free(&int_result);
    return 0;
}