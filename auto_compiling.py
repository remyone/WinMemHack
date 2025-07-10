import os

os.system("gcc -c MT\\func.c -o MT\\func.o")
os.system("gcc -c MemoryScanner\\main.c -I L:\\WinMemHack\\MT -o MemoryScanner\\main.o")
os.system("ar -rcs MT\\libMTlib.a MT\\func.o")
os.system("ar -rcs MemoryScanner\\libMSlib.a MemoryScanner\\main.o");
os.system("gcc main.c -L L:\\WinMemHack\\MT -L L:\\WinMemHack\\MemoryScanner -lMSlib -lMTlib -I L:\\WinMemHack\\MT -I L:\\WinMemHack\\MemoryScanner -o MEU.exe");
