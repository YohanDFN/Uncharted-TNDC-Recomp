@echo off
set PATH=C:\msys64\clang64\bin;%PATH%
title Uncharted 2 Native C++23 Recomp
echo ============================================================
echo   UNCHARTED 2: AMONG THIEVES - STANDALONE NATIVE (C++23)
echo   Zero code machine PS4 execute - 100%% code natif PC
echo   Affichage sur le 2eme ecran - Fermeture auto apres 10s
echo ============================================================
echo.
echo Lancement du moteur natif sur l'ecran 2 (fermeture auto a 10s)...
echo Appuyez sur ECHAP pour quitter a tout moment.
echo.
out\uncharted2-native.exe --game "C:\Users\Yohan\Desktop\Uncharted\CUSA02343" --elf "out\big2\eboot.elf" --display 1 --timeout 10 %*
echo.
echo Fin d'execution du moteur natif. Code de sortie: %ERRORLEVEL%
pause
