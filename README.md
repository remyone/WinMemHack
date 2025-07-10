# WinMemHack 🛠️  
*A low-level Windows process memory manipulation tool*

## Features ✨
- **Process hunting** by name (case-insensitive)
- **Direct memory patching** via `WriteProcessMemory`
- **Process verification** with executable path display
- **Colorized terminal output** for better visibility
- **Error handling** with WinAPI error codes
- **Memory scanner** via `ReadProcessMemory` and `VirtualQueryEx`. **Also there is multi threading**
- Console with different commands:
  - exit
  - set address new_value
  - first_scan value
  - next_scan value
  - new_scan
  - scan_settings
  - clear

## Compiling 🔧
```bash
> gcc -c YourPathTo\WinMemHack\func.c -o MT\func.o
> gcc -c YourPathTo\WinMemHack\MemoryScanner\main.c -I YourPathTo\WinMemHack\MT -o YourPathTo\WinMemHack\MemoryScanner\main.o
> ar -rcs YourPathTo\WinMemHack\MT\libMTlib.a YourPathTo\WinMemHack\MT\func.o
> ar -rcs YourPathTo\WinMemHack\MemoryScanner\libMSlib.a YourPathTo\WinMemHack\MemoryScanner\main.o
> gcc main.c -L YourPathTo\WinMemHack\MT -L YourPathTo\WinMemHack\MemoryScanner -lMSlib -lMTlib -I YourPathTo\WinMemHack\MT -I YourPathTo\WinMemHack\MemoryScanner -o MEU.exe
```
**OR**
### Use auto compiling made by me on python by changing 'L:\\' to your path to WinMemHack. The file 'auto_compiling.py' has to be situated in the folder 'WinMemHack'! Then run the command:
```bash
> python3 auto_compiling.py
```
## Usage 🚀
```bash
> ./MEU.exe ProcessName.exe
```
