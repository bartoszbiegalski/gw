@echo off
if exist build rmdir /s /q build
cmake --preset windows-x64
pause
