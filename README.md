# WinMemHack 🛠️  
*A low-level Windows process memory manipulation tool*

## Features ✨
- **Process hunting** by name (case-insensitive)
- **Direct memory patching** via `WriteProcessMemory`
- **Process verification** with executable path display
- **Colorized terminal output** for better visibility
- **Error handling** with detailed WinAPI error codes
- **Memory scanner** via `ReadProcessMemory` and `VirtualQueryEx`. Also there is multi threading

## Compiling
```bash
> cd MT
> gcc -c func.c -o func.o
> ar -rcs libMTlib.a func.o
> cd ..
> gcc main.c -L YourPathTo\WinMemHack\MT -lMTlib -I YourPathTo\WinMemHack\MT -o MEU.exe
```
## Usage 🚀
```bash
> ./MEU.exe ProcessName.exe
```
