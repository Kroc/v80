@ECHO OFF
CLS & TITLE Building v80...
CD %~dp0
ECHO:

SET "BIN_DIR=bin"

REM # the assembler and linker is WLA-DX
SET WLA_Z80="%BIN_DIR%\wla-dx\wla-z80.exe"  -x -I "src"
SET WLA_LINK="%BIN_DIR%\wla-dx\wlalink.exe" -A -S

REM # RunCPM "emulator"
SET "CPM_DIR=%BIN_DIR%\RunCPM"
SET RUN_CPM="%CPM_DIR%\RunCPM.exe"

IF NOT EXIST "build" MKDIR "build"

%WLA_Z80% -v ^
    -o "build\v80.o" ^
       "v80.wla"

IF ERRORLEVEL 1 EXIT /B 1

%WLA_LINK% -v -b ^
    "src\link_v80.ini" ^
    "build\v80.com"

IF ERRORLEVEL 1 EXIT /B 1

IF NOT EXIST "%CPM_DIR%\A\0" MKDIR "%CPM_DIR%\A\0"
DEL /F /Q "%CPM_DIR%\A\0\*.*"

REM # copy the COM files into the CP/M disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%CPM_DIR%\A\0" /B
REM # copy test v80 files
COPY /N /Y "test\*.v80" /A "%CPM_DIR%\A\0" /A

REM # run the CP/M emulator
START "RunCPM" /D "%CPM_DIR%" %RUN_CPM%