# WinMemHack 🛠️  
*A low-level Windows process memory manipulation tool*

## Features ✨
- **Process hunting** by name (case-insensitive)
- **Direct memory patching** via `WriteProcessMemory`
- **Process verification** with executable path display
- **Colorized terminal output** for better visibility
- **Error handling** with detailed WinAPI error codes

## Usage 🚀
**./main.exe ProcessName.exe Address New_Value**. For example:
```bash
./main.exe game.exe 0xABCDEF42 1337
```
