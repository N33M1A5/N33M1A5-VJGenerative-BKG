@echo off
chcp 65001 >nul
title N33 VJ EXPANDED 1.2.2 - INSTALADOR
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_VJ_EXPANDED_122.ps1"
set "RC=%errorlevel%"
if not "%RC%"=="0" echo Falha na instalacao. Confira a mensagem acima.
echo.
pause
exit /b %RC%
