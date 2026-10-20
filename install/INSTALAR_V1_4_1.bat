@echo off
chcp 65001 >nul
title N33M1A5 VJ Gen V1.4.1
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_V1_4_1.ps1"
set "RC=%errorlevel%"
if not "%RC%"=="0" echo Falha na instalacao. Leia o erro acima.
echo.
pause
exit /b %RC%
