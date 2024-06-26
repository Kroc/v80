@ECHO OFF
CLS & TITLE Building v80...
CD %~dp0
ECHO:

SET "BIN_DIR=bin"

REM # the PC assembler and linker is WLA-DX
SET WLA_Z80="%BIN_DIR%\wla-dx\wla-z80.exe" -x
SET WLA_LINK="%BIN_DIR%\wla-dx\wlalink.exe" -A -S

REM # RunCPM "emulator"
SET "DIR_RUNCPM=%BIN_DIR%\RunCPM"
SET "BIN_RUNCPM=%DIR_RUNCPM%\RunCPM.exe"

REM # NTVCM, CP/M Virtual Machine
REM # https://github.com/davidly/ntvcm
SET "DIR_NTVCM=%BIN_DIR%\ntvcm"
SET BIN_NTVCM=ntvcm.exe -l -p

REM # VBinDiff for visual difference comparing
SET "BIN_VBINDIFF=%BIN_DIR%\vbindiff\VBinDiff.exe"

REM # build v80 assembler from WLA-DX source:
REM # ==========================================================================
IF NOT EXIST "build" MKDIR "build"
DEL /F /Q "build\*.*"

%WLA_Z80% -v ^
    -I "wla" ^
    -o "build\v80.o" ^
       "v80.wla"

IF ERRORLEVEL 1 EXIT /B 1

%WLA_LINK% -v -b ^
    "wla\link.ini" ^
    "build\v80.com"

IF ERRORLEVEL 1 EXIT /B 1

REM # build tests:
REM # ==========================================================================
REM # clear NTVCM cache
DEL /F /Q "%DIR_NTVCM%\*.v80"
DEL /F /Q "%DIR_NTVCM%\*.com"

REM # copy the COM files into the NTVCM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_NTVCM%" /B
REM # copy test v80 files
COPY /N /Y "test\*.v80" /A "%DIR_NTVCM%" /A

REM # clear RunCPM cache
IF NOT EXIST "%DIR_RUNCPM%\A\0" MKDIR "%DIR_RUNCPM%\A\0"
DEL /F /Q "%DIR_RUNCPM%\A\0\*.*"

REM # copy the COM files into the RunCPM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_RUNCPM%\A\0" /B
REM # copy test v80 files
COPY /N /Y "test\*.v80" /A "%DIR_RUNCPM%\A\0" /A

REM # run test.v80 without WLA equivalent
CALL :v80 test
IF ERRORLEVEL 1 EXIT /B %ERRORLEVEL%

REM # do binary comparisons
CALL :RunTest z80
IF ERRORLEVEL 1 EXIT /B %ERRORLEVEL%
CALL :RunTest jr
IF ERRORLEVEL 1 EXIT /B %ERRORLEVEL%

REM # --------------------------------------------------------------------------
REM # if no errors, copy v80 binary to release folder

COPY /N /Y "build\v80.com" /B "release"

ECHO OK.
EXIT /B 0

:RunTest
REM # ==========================================================================
ECHO:

REM # build "%~1.wla" with WLA-DX & "%~1.v80" with v80
CALL :wla %~1
CALL :v80 %~1

REM # compare the two files
FC /B "build\%~1.com" "%DIR_NTVCM%\%~1.com"
IF ERRORLEVEL 1 START "" %BIN_VBINDIFF% "build\%~1.com" "%DIR_NTVCM%\%~1.com"

GOTO:EOF

:wla
REM # assemble with WLA-DX
REM # --------------------------------------------------------------------------
REM # assmeble file to test.o, regardless of input name
%WLA_Z80% ^
    -I "test" ^
    -o "build\test.o" ^
       "test\%~1.wla"

IF ERRORLEVEL 1 EXIT /B %ERRORLEVEL%

REM # link test.o to input named .com file
%WLA_LINK% ^
    -b "test\link.ini" ^
       "build\%~1.com"

REM # return with result of the link
EXIT /B %ERRORLEVEL%


:v80
REM # assemble with V80
REM # --------------------------------------------------------------------------
PUSHD "%DIR_NTVCM%"

%BIN_NTVCM% v80.com %~1.v80

REM # if NTVCM hits a HALT instruction, launch RunCPM
IF ERRORLEVEL 1 POPD & START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM% & EXIT /B 1

POPD
ECHO:

GOTO:EOF
