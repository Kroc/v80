@ECHO OFF
CLS & TITLE Building v80...
CD %~dp0
ECHO:

SET "BIN_DIR=bin"

REM # the PC assembler and linker is WLA-DX
SET WLA_Z80="%BIN_DIR%\wla-dx\wla-z80.exe"  -x -I "src"
SET WLA_LINK="%BIN_DIR%\wla-dx\wlalink.exe" -A -S

REM # RunCPM "emulator"
SET "DIR_RUNCPM=%BIN_DIR%\RunCPM"
SET "BIN_RUNCPM=%DIR_RUNCPM%\RunCPM.exe"

REM # NTVCM, CP/M Virtual Machine
REM # https://github.com/davidly/ntvcm
SET "DIR_NTVCM=%BIN_DIR%\ntvcm"
SET "BIN_NTVCM=ntvcm.exe -l"

IF NOT EXIST "build" MKDIR "build"

%WLA_Z80% -v ^
    -o "build\v80.o" ^
       "v80.wla"

IF ERRORLEVEL 1 EXIT /B 1

%WLA_LINK% -v -b ^
    "src\link_v80.ini" ^
    "build\v80.com"

IF ERRORLEVEL 1 EXIT /B 1

REM # copy the COM files into the NTVCM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_NTVCM%" /B
REM # copy test v80 files
COPY /N /Y "test\*.v80" /A "%DIR_NTVCM%" /A

ECHO:

REM # run NTVCM
PUSHD "%DIR_NTVCM%"

%BIN_NTVCM% v80.com test.v80

REM # if NTVCM does not hit a HALT instruction, do not launch RunCPM
IF %ERRORLEVEL% EQU 0 POPD & ECHO: & ECHO OK. & EXIT /B 0

POPD
ECHO:

IF NOT EXIST "%DIR_RUNCPM%\A\0" MKDIR "%DIR_RUNCPM%\A\0"
DEL /F /Q "%DIR_RUNCPM%\A\0\*.*"

REM # copy the COM files into the RunCPM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_RUNCPM%\A\0" /B
REM # copy test v80 files
COPY /N /Y "test\*.v80" /A "%DIR_RUNCPM%\A\0" /A

REM # run the CP/M emulator
START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM%

