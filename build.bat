@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\bin\vcvars32.bat"

"C:\Program Files (x86)\Git\bin\sh.exe" --login -c "git pull -v --progress origin develop"
IF ERRORLEVEL 1 GOTO errorHandling

devenv PumpkinLog.sln /Rebuild "Release|Win32"
IF ERRORLEVEL 1 GOTO errorHandling

del "Z:\www\default\data\PumpkinLog.zip"
cd Release_Win32
"C:\Program Files\7-Zip\7z" a -tzip "Z:\www\default\data\PumpkinLog.zip" "PumpkinLog.exe" "libPumpkinLog.h" "libPumpkinLog.lib"

exit

:errorHandling
exit /b 1
