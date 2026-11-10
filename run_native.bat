@echo off
set PATH=C:\msys64\clang64\bin;%PATH%
title Uncharted 2 Native C++23 Recomp
echo ============================================================
echo   UNCHARTED 2: AMONG THIEVES - STANDALONE NATIVE (C++23)
echo   Zero code machine PS4 execute - 100%% code natif PC
echo ============================================================
echo.
echo Appuyez sur ECHAP ou fermez la fenetre pour quitter.
echo.
out\uncharted2-native.exe --game "C:\Users\Yohan\Desktop\Uncharted\CUSA02343" --elf "out\big2\eboot.elf"
echo.
echo Fin d'execution. Code: %ERRORLEVEL%
pause
