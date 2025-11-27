set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(MINGW_PREFIX "C:/msys64/mingw64")

set(CMAKE_C_COMPILER "${MINGW_PREFIX}/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "${MINGW_PREFIX}/bin/g++.exe")
set(CMAKE_RC_COMPILER "${MINGW_PREFIX}/bin/windres.exe")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -m64")

set(MSYS_INCLUDE "${MINGW_PREFIX}/include")
set(MSYS_LIB "${MINGW_PREFIX}/lib")