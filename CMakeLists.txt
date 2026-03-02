cmake_minimum_required(VERSION 4.3.0)
project(WinMemHack LANGUAGES C)

set(LibrarySources
        src/MemoryScanner.c
        src/MemoryThread.c
)

set(LibraryHeaders
        include/MemoryScanner.h
        include/MemoryThread.h
        include/initVFile.h
)

add_library(LIB_WinMemHack STATIC ${LibrarySources} ${LibraryHeaders})
target_include_directories(LIB_WinMemHack PUBLIC include)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE LIB_WinMemHack)
target_include_directories(${PROJECT_NAME} PRIVATE include)