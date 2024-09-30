@ECHO OFF
CLS & TITLE Building v80...
CD %~dp0
ECHO:

SET "BIN_DIR=bin"

REM # the v0 (bootstrap) PC assembler and linker is WLA-DX
SET WLA_Z80="%BIN_DIR%\wla-dx\wla-z80.exe" -x
SET WLA_6502="%BIN_DIR%\wla-dx\wla-6502.exe" -x
SET WLA_LINK="%BIN_DIR%\wla-dx\wlalink.exe" -A -S
SET WLA_DEBUG=-DDEBUG=1

REM # RunCPM "emulator"
SET "DIR_RUNCPM=%BIN_DIR%\RunCPM"
SET "BIN_RUNCPM=%DIR_RUNCPM%\RunCPM.exe"

REM # NTVCM, CP/M Virtual Machine
REM # https://github.com/davidly/ntvcm
SET "DIR_NTVCM=%BIN_DIR%\ntvcm"
SET BIN_NTVCM=ntvcm.exe -l -p

REM # VBinDiff for visual difference comparing
SET "BIN_VBINDIFF=%BIN_DIR%\vbindiff\VBinDiff.exe"

REM # build v80 assembler [v0] from WLA-DX source:
REM # ==========================================================================
IF NOT EXIST "build" MKDIR "build"
DEL /F /Q "build\*.*"

%WLA_Z80% -v ^
    -I "v0" ^
    -o "build\v80.o" ^
       "v80.wla"

IF ERRORLEVEL 1 GOTO:ERR

%WLA_LINK% -v -b ^
    "v0\link.ini" ^
    "build\v80.com"

IF ERRORLEVEL 1 GOTO:ERR
ECHO:

REM # build & run tests:
REM # --------------------------------------------------------------------------
REM # clear NTVCM cache
DEL /F /Q "%DIR_NTVCM%\*.v??"  >NUL 2>&1
DEL /F /Q "%DIR_NTVCM%\*.com"  >NUL 2>&1
DEL /F /Q "%DIR_NTVCM%\*.prg"  >NUL 2>&1
DEL /F /Q "%DIR_NTVCM%\*.sym"  >NUL 2>&1

REM # copy the COM files into the NTVCM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_NTVCM%" /B  >NUL
REM # copy test v80/v65 files
COPY /N /Y "test\*.v??"  /A "%DIR_NTVCM%" /A   >NUL

REM # clear RunCPM cache
IF NOT EXIST "%DIR_RUNCPM%\A\0" MKDIR "%DIR_RUNCPM%\A\0"  >NUL
DEL /F /Q "%DIR_RUNCPM%\A\0\*.*"  >NUL 2>&1

REM # copy the COM files into the RunCPM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "build\*.com" /B "%DIR_RUNCPM%\A\0" /B  >NUL
REM # copy test v80/v65 files
COPY /N /Y "test\*.v??"  /A "%DIR_RUNCPM%\A\0" /A  >NUL

REM # run "test.v80" without WLA equivalent
CALL :v80_z80 test

REM # do binary comparisons:
REM # - all Z80 instructions
CALL :RunTestZ80 z80
REM # - relative jump distances
CALL :RunTestZ80 jr

REM DEL /F /Q "%BIN_DIR%\agon\sdcard\v80\*.*"
REM COPY /N /Y "release\v80.com" /B "%BIN_DIR%\agon\sdcard\v80\V80.COM" /B
REM COPY /N /Y "v1\*.v80" /A "%BIN_DIR%\agon\sdcard\v80\*.v80" /A
REM 
REM PUSHD "%BIN_DIR%\agon"
REM "fab-agon-emulator.exe" --scale integer --mode 3
REM POPD & EXIT

REM # if no errors, use v80 to assemble itself
REM # ==========================================================================

REM # copy v80 [v1] source into NTVCM directory
COPY /N /Y "v1\*.v??" /B "%DIR_NTVCM%" /A  >NUL
REM # and RunCPM
COPY /N /Y "v1\*.v??" /A "%DIR_RUNCPM%\A\0" /A  >NUL

REM # do a 1st-generation build of v80 [v1] using v80 [v0]!
CALL :v80_z80   cpm_z80 v80.com

REM # do binary comparisons:
REM # - all Z80 instructions
CALL :RunTestZ80 z80

REM # do a 2nd-generation build of v80, i.e. v80 [v1] building v80 [v1]
CALL :v80_z80   cpm_z80 v80_2nd.com

REM # compare 1st and 2nd generation builds
FC /B "%DIR_NTVCM%\v80.com" "%DIR_NTVCM%\v80_2nd.com"  >NUL
IF ERRORLEVEL 1 START "" %BIN_VBINDIFF% "%DIR_NTVCM%\v80.com"

REM # 6502:
REM # ==========================================================================
REM # use v80 to assemble a version of v80 that assembles 6502 code...
CALL :v80_z80   cpm_6502.v80 v65.com

COPY /N /Y "%DIR_NTVCM%\v65.com" /B "%DIR_RUNCPM%\A\0" /B  >NUL

REM # verify 6502 assembling
CALL :RunTest6502 6502

REM # ==========================================================================
REM # if no errors, copy v80 binary to release folder

ECHO * Populate release folder...
COPY /N /Y "%DIR_NTVCM%\v80.com" /B "release\v80.com" /B  >NUL

ECHO [OK.]
GOTO :END

REM ////////////////////////////////////////////////////////////////////////////

:RunTestZ80
REM # ==========================================================================
REM # run a comparison between WLA-DX-Z80 & V80-Z80
REM # --------------------------------------------------------------------------

REM # build "%~1.wla" with WLA-DX & "%~1.v80" with v80
CALL :wla_z80 %~1
CALL :v80_z80 %~1

REM # compare the two files
FC /B "build\%~1.com" "%DIR_NTVCM%\%~1.com"  >NUL
IF ERRORLEVEL 1 START "" %BIN_VBINDIFF% "build\%~1.com" "%DIR_NTVCM%\%~1.com" & GOTO:ERR

GOTO:EOF

:wla_z80
REM # ==========================================================================
REM # assemble with WLA-DX-Z80
REM # --------------------------------------------------------------------------
ECHO * wla-z80.exe %~1 %~2

REM # assmeble file to test.o, regardless of input name
%WLA_Z80% ^
    -I "test" ^
    -o "build\test.o" ^
       "test\%~1.wla"

IF ERRORLEVEL 1 GOTO:ERR

REM # link test.o to input named .com file
%WLA_LINK% ^
    -b "test\link.ini" ^
       "build\%~1.com"

IF ERRORLEVEL 1 GOTO:ERR
GOTO:EOF

:v80_z80
REM # ==========================================================================
REM # assemble with v80-Z80
REM # --------------------------------------------------------------------------
PUSHD "%DIR_NTVCM%"

ECHO * v80.com %~1 %~2
%BIN_NTVCM% v80.com %~1 %~2 > %~n1.sym
IF ERRORLEVEL 1 TYPE %~n1.sym

REM # if NTVCM hits a HALT instruction, launch RunCPM
IF %ERRORLEVEL% EQU -1 GOTO :runCPM
IF ERRORLEVEL 1 GOTO:ERR

POPD
GOTO:EOF

:runCPM
REM # --------------------------------------------------------------------------
POPD
COPY /N /Y "%DIR_NTVCM%\v80.com" /B "%DIR_RUNCPM%\A\0\v80.com" /B  >NUL
START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM% & GOTO:ERR
GOTO:ERR

:RunTest6502
REM # ==========================================================================
REM # run a comparison between WLA-DX-6502 & V80-6502
REM # --------------------------------------------------------------------------

REM # build "%~1.wla" with WLA-DX-6502 & "%~1.v65" with v80-6502
CALL :wla_6502 %~1
CALL :v80_6502 %~1

REM # compare the two files
FC /B "build\%~1.prg" "%DIR_NTVCM%\%~1.prg"  >NUL
IF ERRORLEVEL 1 START "" %BIN_VBINDIFF% "build\%~1.prg" "%DIR_NTVCM%\%~1.prg" & GOTO:ERR

GOTO:EOF

:wla_6502
REM # ==========================================================================
REM # assemble with WLA-DX-6502
REM # --------------------------------------------------------------------------
REM # assmeble file to test.o, regardless of input name
%WLA_6502% ^
    -I "test" ^
    -o "build\test.o" ^
       "test\%~1.wla"

IF ERRORLEVEL 1 GOTO:ERR

REM # link test.o to input-named .prg file
%WLA_LINK% ^
    -b "test\link.ini" ^
       "build\%~1.prg"

IF ERRORLEVEL 1 GOTO:ERR
GOTO:EOF

:v80_6502
REM # ==========================================================================
REM # assemble with v65
REM # --------------------------------------------------------------------------
PUSHD "%DIR_NTVCM%"

ECHO * v65.com %~1 %~2
%BIN_NTVCM% v65.com %~1 %~2  > %~n1.sym
IF ERRORLEVEL 1 TYPE %~n1.sym

REM # if NTVCM hits a HALT instruction, launch RunCPM
IF %ERRORLEVEL% EQU -1 POPD & START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM% & GOTO:ERR
IF ERRORLEVEL 1 GOTO:ERR

POPD
ECHO:
GOTO:EOF

:ERR
ECHO ! ERROR
EXIT 1

:END
EXIT 0