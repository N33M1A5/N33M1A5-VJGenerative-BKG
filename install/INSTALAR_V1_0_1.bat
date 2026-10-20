@echo off
chcp 65001 >nul
setlocal
title N33M1A5 VJ Generative 1.0.1
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSTALAR_V1_0_1.ps1"
set "CODE=%errorlevel%"
if not "%CODE%"=="0" echo.
if not "%CODE%"=="0" echo ERRO AO INSTALAR. Confira o texto acima e feche o Resolume.
echo.
pause
exit /b %CODE%
