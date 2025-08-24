@echo off

cd /d %~dp0
@REM :LOOP
echo.
echo Starting Build ...
echo.

echo BUILDING: icon ...
windres icon.rc -o icon.o

echo BUILDING: cal.exe ...
gcc -O3 -march=native -flto -ffunction-sections -fdata-sections -Wl,--gc-sections -s cal.c icon.o -o cal.exe

echo.
echo Cleaning ...
del /q icon.o

echo.
echo Task Completed (Press any key to exit) ...
pause >nul 2>&1
exit

@REM goto LOOP









