@echo off
chcp 65001 >nul
title N33M1A5 VJ GENERATIVE UNIFIED V1.3
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_UNIFIED_V1_3.ps1"
set "RC=%errorlevel%"
if not "%RC%"=="0" echo ERRO! Revise a mensagem acima e confira o fechamento do Resolume.
echo.
pause
exit /b %RC%
