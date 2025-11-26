set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86)

set(MINGW32_PREFIX "C:/msys64/mingw32")

set(CMAKE_C_COMPILER "${MINGW32_PREFIX}/bin/i686-w64-mingw32-gcc.exe")
set(CMAKE_CXX_COMPILER "${MINGW32_PREFIX}/bin/i686-w64-mingw32-g++.exe")
set(CMAKE_RC_COMPILER "${MINGW32_PREFIX}/bin/windres.exe")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -m32")