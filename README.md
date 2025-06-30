# WinMemHack 🛠️  
*A low-level Windows process memory manipulation tool*

## Features ✨
- **Process hunting** by name (case-insensitive)
- **Direct memory patching** via `WriteProcessMemory`
- **Process verification** with executable path display
- **Colorized terminal output** for better visibility
- **Error handling** with WinAPI error codes
- **Memory scanner** via `ReadProcessMemory` and `VirtualQueryEx`. **Also there is multi threading**

## Compiling 🔧
```bash
> gcc -c YourPathTo\MemoryEditorUtility\func.c -o MT\func.o
> gcc -c YourPathTo\MemoryEditorUtilityMemoryScanner\main.c -I YourPathTo\MemoryEditorUtility\MT -o YourPathTo\MemoryEditorUtility\MemoryScanner\main.o
> ar -rcs YourPathTo\MemoryEditorUtility\MT\libMTlib.a YourPathTo\MemoryEditorUtility\MT\func.o
> ar -rcs YourPathTo\MemoryEditorUtility\MemoryScanner\libMSlib.a YourPathTo\MemoryEditorUtility\MemoryScanner\main.o
> gcc main.c -L YourPathTo\MemoryEditorUtility\MT -L YourPathTo\MemoryEditorUtility\MemoryScanner -lMSlib -lMTlib -I YourPathTo\MemoryEditorUtility\MT -I YourPathTo\MemoryEditorUtility\MemoryScanner -o MEU.exe
```
## Usage 🚀
```bash
> ./MEU.exe ProcessName.exe
```
