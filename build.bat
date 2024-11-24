@ECHO OFF
CLS & TITLE Building v80...
CD %~dp0
ECHO:

SET DIR_BIN=%~dp0bin

REM # the v0 (bootstrap) PC assembler and linker is WLA-DX:
REM # this is our "source of ground truth" to ensure v80
REM # is assembling the correct opcodes etc.
SET WLA_Z80="%DIR_BIN%\wla-dx\wla-z80.exe" -x
SET WLA_6502="%DIR_BIN%\wla-dx\wla-6502.exe" -x
SET WLA_LINK="%DIR_BIN%\wla-dx\wlalink.exe" -A -S
SET WLA_DEBUG=-DDEBUG=1

REM # RunCPM "emulator"
SET DIR_RUNCPM=%DIR_BIN%\RunCPM
SET BIN_RUNCPM="%DIR_RUNCPM%\RunCPM.exe"

REM # NTVCM, CP/M Virtual Machine
REM # https://github.com/davidly/ntvcm
SET DIR_NTVCM=%DIR_BIN%\ntvcm
SET BIN_NTVCM="%DIR_NTVCM%\ntvcm.exe" -l -p

REM # VBinDiff for visual difference comparing
REM # https://www.cjmweb.net/vbindiff/
SET BIN_VBINDIFF="%DIR_BIN%\vbindiff\VBinDiff.exe"

REM # create / clean build directory
SET DIR_BUILD=%~dp0build
IF NOT EXIST "%DIR_BUILD%" MKDIR "%DIR_BUILD%"
DEL /F /Q "%DIR_BUILD%\*.*"
REM # clear RunCPM cache
IF NOT EXIST "%DIR_RUNCPM%\A\0" MKDIR "%DIR_RUNCPM%\A\0"  >NUL
DEL /F /Q "%DIR_RUNCPM%\A\0\*.*"  >NUL 2>&1

REM # build v80 assembler [v0] from WLA-DX source:
REM # ==========================================================================
REM # this builds "v80_wla.com", the WLA version of v80

%WLA_Z80% -v ^
    -I "v0" ^
    -o "%DIR_BUILD%\v80_wla.o" ^
       "v80.wla"

IF ERRORLEVEL 1 GOTO:ERR

%WLA_LINK% -v -b ^
    "v0\link.ini" ^
    "%DIR_BUILD%\v80_wla.com"

IF ERRORLEVEL 1 GOTO:ERR
ECHO:

REM # build & run tests:
REM # --------------------------------------------------------------------------
SET DIR_TEST=%~dp0test

REM # copy test v80/v65 files to build directory for running tests
COPY /N /Y "%DIR_TEST%\*.v??"  /A "%DIR_BUILD%" /A   >NUL

REM # copy the COM files into the RunCPM disk directory
REM # "/N" forces an 8.3 file-name in the destination
COPY /N /Y "%DIR_BUILD%\*.com" /B "%DIR_RUNCPM%\A\0" /B  >NUL
REM # copy test v80/v65 files
COPY /N /Y "%DIR_TEST%\*.v??"  /A "%DIR_RUNCPM%\A\0" /A  >NUL

REM # assemble "test.v80" using the WLA-built [v0] version of v80
CALL :v80_wla "test.v80"

REM # do binary comparisons:
REM # - all Z80 instructions
CALL :RunTestWLA z80
REM # - relative jump distances
CALL :RunTestWLA jr

REM DEL /F /Q "%DIR_BIN%\agon\sdcard\v80\*.*"
REM COPY /N /Y "release\v80.com" /B "%DIR_BIN%\agon\sdcard\v80\V80.COM" /B
REM COPY /N /Y "v1\*.v80" /A "%DIR_BIN%\agon\sdcard\v80\*.v80" /A
REM 
REM PUSHD "%DIR_BIN%\agon"
REM "fab-agon-emulator.exe" --scale integer --mode 3
REM POPD & EXIT

REM # if no errors, use v80 to assemble itself
REM # ==========================================================================
REM # copy v80 [v1] source into build directory
COPY /N /Y "v1\*.v??" /B "%DIR_BUILD%" /A  >NUL
REM # and to RunCPM
COPY /N /Y "v1\*.v??" /A "%DIR_RUNCPM%\A\0" /A  >NUL

REM # do a 1st-generation build of v80 [v1] using v80 [v0]!
CALL :v80_wla "cpm_z80.v80" "v80.com"

REM # do binary comparisons:
REM # - all Z80 instructions
CALL :RunTestZ80 z80

REM REM # do a 2nd-generation build of v80, i.e. v80 [v1] building v80 [v1]
REM CALL :v80_z80   cpm_z80 v80_2nd.com
REM 
REM REM # compare 1st and 2nd generation builds
REM FC /B "%DIR_BUILD%\v80.com" "%DIR_BUILD%\v80_2nd.com"  >NUL
REM IF ERRORLEVEL 1 START "" %BIN_VBINDIFF% "%DIR_BUILD%\v80.com"

REM # 6502:
REM # ==========================================================================
REM # use v80 to assemble a version of v80 that assembles 6502 code...
CALL :v80 "cpm_6502.v80" "v65.com"

REM # copy v65 to RunCPM's directory
COPY /N /Y "%DIR_BUILD%\v65.com" /B "%DIR_RUNCPM%\A\0" /B  >NUL

REM # verify 6502 assembling
CALL :RunTest6502 6502

REM # build v80 [v2]:
REM # ==========================================================================
REM # copy v2 code to build directory
COPY /N /Y "v2\*.v??" /B "%DIR_BUILD%" /A  >NUL
REM # and to RunCPM
COPY /N /Y "v2\*.v??" /A "%DIR_RUNCPM%\A\0" /A  >NUL

REM # use v80 [v1] to build v80 [v2]
CALL :v80_wla "cpm_z80.v80" "v80.com"
REM CALL :v80 "cpm_z80.v80" "v80.com"

REM # ==========================================================================
REM # if no errors, copy v80 binary to release folder

ECHO * Populate release folder...
COPY /N /Y "%DIR_BUILD%\v*.com" /B "release\v*.com" /B  >NUL

ECHO [OK.]
GOTO :END

REM ////////////////////////////////////////////////////////////////////////////

:RunTestWLA
REM # ==========================================================================
REM # run a comparison between WLA-DX-Z80 [PC] & v80_wla [v0]
REM # --------------------------------------------------------------------------

REM # build "%~1.wla" with WLA-DX [PC],
CALL :wla_z80   %~1
REM # and "%~1.v80" with v80_wla [v0]
CALL :v80_wla   %~1.v80

REM # compare the two files:
FC /B ^
    "%DIR_BUILD%\%~1_wla.com" ^
    "%DIR_BUILD%\%~1.com"  >NUL

IF ERRORLEVEL 1 ^
    START "" %BIN_VBINDIFF% ^
        "%DIR_BUILD%\%~1_wla.com" ^
        "%DIR_BUILD%\%~1.com" ^
    & GOTO:ERR

GOTO:EOF

:RunTestZ80
REM # ==========================================================================
REM # run a comparison between WLA-DX-Z80 [PC] & v80 [v1]
REM # --------------------------------------------------------------------------

REM # build "%~1.wla" with WLA-DX & "%~1.v80" with v80
CALL :wla_z80   %~1
CALL :v80       %~1.v80

REM # compare the two files
FC /B ^
    "%DIR_BUILD%\%~1_wla.com" ^
    "%DIR_BUILD%\%~1.com"  >NUL

IF ERRORLEVEL 1 ^
    START "" %BIN_VBINDIFF% ^
        "%DIR_BUILD%\%~1_wla.com" ^
        "%DIR_BUILD%\%~1.com" ^
    & GOTO:ERR

GOTO:EOF

:wla_z80
REM # ==========================================================================
REM # assemble .wla source with WLA-DX-Z80
REM # --------------------------------------------------------------------------
ECHO * wla-z80.exe %~1.wla

REM # assmeble file to test.o, regardless of input name
%WLA_Z80% ^
    -I "%DIR_TEST%" ^
    -o "%DIR_BUILD%\test.o" ^
       "%DIR_TEST%\%~1.wla"

IF ERRORLEVEL 1 GOTO:ERR

REM # link test.o to input-named *_wla.com file
%WLA_LINK% ^
    -b "%DIR_TEST%\link.ini" ^
       "%DIR_BUILD%\%~1_wla.com"

IF ERRORLEVEL 1 GOTO:ERR
GOTO:EOF

:v80_wla
REM # ==========================================================================
REM # assemble a .v80 file using the WLA-built [v0] version of v80
REM # --------------------------------------------------------------------------
ECHO * v80_wla.com %~1 %~2
PUSHD "%DIR_BUILD%"

%BIN_NTVCM% v80_wla.com %~1 %~2 > %~n1.sym
IF ERRORLEVEL 1 TYPE %~n1.sym

REM # if NTVCM hits a HALT instruction, launch RunCPM
REM # TODO: prefill the input buffer with the same invocation used
IF %ERRORLEVEL% EQU -1 GOTO :runCPM
IF ERRORLEVEL 1 GOTO:ERR

POPD
GOTO:EOF

:v80
REM # ==========================================================================
REM # assemble with v80
REM # --------------------------------------------------------------------------
ECHO * v80.com %~1 %~2
PUSHD "%DIR_BUILD%"

%BIN_NTVCM% v80.com %~1 %~2 > %~n1.sym
IF ERRORLEVEL 1 TYPE %~n1.sym

REM # if NTVCM hits a HALT instruction, launch RunCPM
IF %ERRORLEVEL% EQU -1 GOTO :runCPM
IF ERRORLEVEL 1 GOTO:ERR

POPD
GOTO:EOF

:runCPM
REM # --------------------------------------------------------------------------
TYPE %~n1.sym
POPD
START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM%
GOTO:ERR

:RunTest6502
REM # ==========================================================================
REM # run a comparison between WLA-DX-6502 & v65
REM # --------------------------------------------------------------------------
REM # build "%~1.wla" with WLA-DX-6502 [PC],
CALL :wla_6502  %~1
REM # and "%~1.v65" with v65
CALL :v65       %~1.v65

REM # compare the two files:
FC /B ^
    "%DIR_BUILD%\%~1_wla.prg" ^
    "%DIR_BUILD%\%~1.prg"  >NUL

IF ERRORLEVEL 1 ^
    START "" %BIN_VBINDIFF% ^
        "%DIR_BUILD%\%~1_wla.prg" ^
        "%DIR_BUILD%\%~1.prg" ^
    & GOTO:ERR

GOTO:EOF

:wla_6502
REM # ==========================================================================
REM # assemble with WLA-DX-6502
REM # --------------------------------------------------------------------------
ECHO * wla-6502.exe %~1.wla

REM # assmeble file to test.o, regardless of input name
%WLA_6502% ^
    -I "%DIR_TEST%" ^
    -o "%DIR_BUILD%\test.o" ^
       "%DIR_TEST%\%~1.wla"

IF ERRORLEVEL 1 GOTO:ERR

REM # link test.o to input-named *_wla.prg file
%WLA_LINK% ^
    -b "%DIR_TEST%\link.ini" ^
       "%DIR_BUILD%\%~1_wla.prg"

IF ERRORLEVEL 1 GOTO:ERR
GOTO:EOF

:v65
REM # ==========================================================================
REM # assemble with v65
REM # --------------------------------------------------------------------------
ECHO * v65.com %~1 %~2
PUSHD "%DIR_BUILD%"

%BIN_NTVCM% v65.com %~1 %~2  > %~n1.sym
IF ERRORLEVEL 1 TYPE %~n1.sym

REM # if NTVCM hits a HALT instruction, launch RunCPM
IF %ERRORLEVEL% EQU -1 POPD & START "RunCPM" /D "%DIR_RUNCPM%" %BIN_RUNCPM% & GOTO:ERR
IF ERRORLEVEL 1 GOTO:ERR

POPD
ECHO:
GOTO:EOF

REM ////////////////////////////////////////////////////////////////////////////

:ERR
ECHO ! ERROR
EXIT 1

:END
EXIT 0