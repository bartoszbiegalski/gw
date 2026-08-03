@echo off
if exist build32 rmdir /s /q build32
cmake --preset windows-x86
pause

