# WinMemHack 🛠️  
> A low-level Windows process memory manipulation tool

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
> git clone https://github.com/remyone/WinMemHack.git
> cd WinMemHack/
> mkdir build
> cd build/
> cmake -DCMAKE_BUILD_TYPE=Release ..
> cmake --build . --config Release
```
## Usage 🚀
```bash
> MEU.exe ProcessName.exe
```
