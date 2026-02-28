import os

print("Stage #1")
os.system("gcc -c MemoryScanner\\main.c -I MT -I . -o MemoryScanner\\main.o")
print("Stage #2")
os.system("gcc -c .\\MT\\func.c -I . -o .\\MT\\func.o ")
print("Stage #3")
os.system("ar -rcs .\\MemoryScanner\\libMSlib.a .\\MemoryScanner\\main.o ")
print("Stage #4")
os.system("ar -rcs .\\MT\\libMTlib.a .\\MT\\func.o ")
print("Stage #5")
os.system("gcc main.c -L MT\\ -L MemoryScanner\\ -lMSlib -lMTlib -I MT\\ -I MemoryScanner\\ -I . -o MEU.exe")