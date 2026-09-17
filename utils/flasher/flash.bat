@echo off

REM SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
REM SPDX-License-Identifier: GPL-3.0-or-later

setlocal EnableExtensions
cd /d "%~dp0"

REM ESP Unchained Bluetooth Controller
REM Firmware flasher script

REM Colors
for /f %%E in ('echo prompt $E ^| cmd') do set "ESC=%%E"
set "BT=%ESC%[38;2;0;130;252m"
set "WHITE=%ESC%[1;37m"
set "RST=%ESC%[0m"
if defined NO_COLOR ( set "BT=" & set "WHITE=" & set "RST=" )

REM Banner
:: _   _         _         _             _
::| | | |_ _  __| |_  __ _(_)_ _  ___ __| |
::| |_| | ' \/ _| ' \/ _` | | ' \/ -_) _` |
:: \___/|_||_\__|_||_\__,_|_|_||_\___\__,_|
::  ___ _          _            _   _
:: | _ ) |_  _ ___| |_ ___  ___| |_| |_
:: | _ \ | || / -_)  _/ _ \/ _ \  _| ' \
:: |___/_|\_,_\___|\__\___/\___/\__|_||_|
setlocal EnableDelayedExpansion
<nul set /p "=%BT%"
for /f "delims=" %%L in ('findstr /b /c:"::" "%~f0"') do (set "line=%%L" & echo(!line:~2!)
<nul set /p "=%RST%"
echo(
echo(%WHITE%            Flasher tool%RST%
echo(
endlocal

set "ERR=0"

REM Locate the bundled espflash tool
set "TOOL=%~dp0tools\windows-x86_64\espflash.exe"
if not exist "%TOOL%" ( echo [!] espflash tool not found: "%TOOL%" & set "ERR=1" & goto :end )
"%TOOL%" --version >nul 2>&1
if errorlevel 1 ( echo [!] Could not execute the espflash tool. & set "ERR=1" & goto :end )
echo [+] Using espflash tool: %TOOL%

REM Pick the first firmware image (.bin) found next to this script
set "FW="
set "FWNAME="
for /f "delims=" %%F in ('dir /b /a-d /o:n "%~dp0*.bin" 2^>nul') do if not defined FW ( set "FWNAME=%%F" & set "FW=%~dp0%%F" )
if not defined FW ( echo [!] No firmware ^(.bin^) image found next to this script. & set "ERR=1" & goto :end )
echo [+] Firmware image: %FWNAME%

REM Prompt the user to plug in their device if they haven't already
set /p "_=Connect ONLY the board you want to flash, then press Enter to continue... "

REM Print device information (also confirms the board is reachable)
echo [*] Target device:
"%TOOL%" board-info %*
if errorlevel 1 ( echo [!] Could not read device information. Is the board connected? If several devices are connected, pass the port, e.g.: flash.bat --port COM5 & set "ERR=1" & goto :end )

REM Ask for the user's confirmation before proceeding with the flash
set "CONFIRM="
set /p "CONFIRM=Are you sure you want to proceed with flashing? (y/N) "
if /i not "%CONFIRM%"=="y" ( echo Aborted. & set "ERR=1" & goto :end )

REM Flash
"%TOOL%" write-bin %* 0x0 "%FW%"
if errorlevel 1 ( echo [!] Flashing FAILED - see the messages above. & set "ERR=1" & goto :end )
echo Done. You can unplug the device.

:end
echo(
pause
exit /b %ERR%
