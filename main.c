#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string.h>

#define DT "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[94m"

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
        printf("------------------------------------------------------------------------------\n");
        printf(BLUE"| Process name: %s |\n"DT, processName);
        printf("------------------------------------------------------------------------------\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf(RED"Usage: ./main ProcessName.exe Address New_Value\n"DT);
        exit(1);
    }

    DWORD PID = _findProcessPidByName(argv[1]);
    uintptr_t address = strtoull(argv[2], NULL, 16);
    int new_value = atoi(argv[3]);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
                                  FALSE,
                                  PID);



    if (hProcess == NULL) {
        printf(RED"HANDLE ERROR %d\n"DT, GetLastError());
        exit(1);
    }

    PrintProcessName(hProcess);
    
    printf(GREEN"Process has been opened successfully!\n"DT);

    if (WriteProcessMemory(hProcess, (LPVOID) address, &new_value, sizeof(new_value), NULL))
        printf(GREEN"Value updated!\n"DT);
    else
        printf(RED"Write failed. Error: %d\n"DT, GetLastError());

    CloseHandle(hProcess);
    return 0;
}