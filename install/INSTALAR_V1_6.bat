@echo off
chcp 65001 >nul
title N33M1A5 VJ Generative V1.6
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_V1_6.ps1"
set "RC=%errorlevel%"
if not "%RC%"=="0" echo ERRO. Veja a mensagem acima.
echo.
pause
exit /b %RC%
