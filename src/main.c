#include "MemoryScanner.h"

#include <stdlib.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string.h>
#include <memoryapi.h>
#include <process.h>

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

enum value_type val_type = BYTES_4;
scan_settings default_sc_settings = {0x0, 0x3FFFFFFFFFFF, BYTES_4};
bool isFirstScanUsed = 0;

void print_scan_settings() {
    printf(BLUE"------------------------\n"DT);

    printf("Start : [%llx]\nStop : [%llx]\n", default_sc_settings.start, default_sc_settings.stop);
    if (default_sc_settings.value_type == BYTES_4)
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

void parse_commands(char buffer[1024], HANDLE hProcess, mem_int_scan_result **int_result) {
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

    if (strcmp(tokens[0], "exit") == 0) {
        destroy_misr(int_result);
        CloseHandle(hProcess);
        exit(0);
    }
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
        scan_memory(hProcess, value, int_result, &isFirstScanUsed);
    } else if (strcmp(tokens[0], "new_scan") == 0) {
        destroy_misr(int_result);
        isFirstScanUsed = 0;
    } else if (strcmp(tokens[0], "next_scan") == 0) {
        unsigned long new_value = strtoul(tokens[1], NULL, 10);
        next_scan_memory(hProcess, new_value, int_result, &isFirstScanUsed);
    }
    else if (strcmp(tokens[0], "scan_settings") == 0)
        print_scan_settings();
    else if (strcmp(tokens[0], "edit") == 0 && strcmp(tokens[1], "scan_settings") == 0) {
        if (strcmp(tokens[2], "start") == 0)
            default_sc_settings.start = strtoull(tokens[3], NULL, 16);
        else if (strcmp(tokens[2], "stop") == 0)
            default_sc_settings.stop = strtoull(tokens[3], NULL, 16);
        else if (strcmp(tokens[2], "value_type") == 0)
            default_sc_settings.value_type = atoi(tokens[3]);
        
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
    printf(GREEN"| MEMORY EDITOR v1.4         |\n"DT);  
    printf(GREEN" ----------------------------\n"DT); 

    DWORD PID = _findProcessPidByName(argv[1]);

    if (PID == -1) {
        printf(RED"PID hasn't been found\n"DT);
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION,
                                  FALSE,
                                  PID);


    if (hProcess == NULL) {
        printf(RED"HANDLE ERROR %d\n"DT, GetLastError());
        return 1;
    }

    mem_int_scan_result *int_result = NULL;;

    char buffer[1024];

    PrintProcessName(hProcess);

    printf(GREEN"Process has been opened successfully!\n"DT);

    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        parse_commands(buffer, hProcess, &int_result);
    }

    return 0;
}
