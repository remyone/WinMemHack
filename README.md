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
> gcc -c MT\func.c -o MT\func.o
> gcc -c MemoryScanner\main.c -I L:\MemoryEditorUtility\MT -o MemoryScanner\main.o
> ar -rcs MT\libMTlib.a MT\func.o
> ar -rcs MemoryScanner\libMSlib.a MemoryScanner\main.o
> gcc main.c -L L:\MemoryEditorUtility\MT -L L:\MemoryEditorUtility\MemoryScanner -lMSlib -lMTlib -I L:\MemoryEditorUtility\MT -I L:\MemoryEditorUtility\MemoryScanner -o MEU.exe
```
## Usage 🚀
```bash
> ./MEU.exe ProcessName.exe
```
