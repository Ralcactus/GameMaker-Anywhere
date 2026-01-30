@echo off
cd /d "%~dp0gfx"
"C:\devkitPro\tools\bin\tex3ds.exe" -i sprites.t3s -o sprites.t3x
echo Done.
pause
