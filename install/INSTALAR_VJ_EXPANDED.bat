@echo off
chcp 65001 >nul
title N33 VJ EXPANDED V1.2
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_VJ_EXPANDED.ps1"
set "RC=%errorlevel%"
if not "%RC%"=="0" echo ERRO AO INSTALAR. Leia a mensagem acima.
echo.
pause
exit /b %RC%
