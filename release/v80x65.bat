@REM # this batch file makes it easier to invoke v80x65;
@REM # CP/M doesn't support directories so we must always invoke NTVCM+v80x65
@REM # from the current working directory -- if it isn't already, we change
@REM # the directory to the same one as this script
@PUSHD %~dp0
@ntvcm.exe v80x65.com %*
@POPD & EXIT /B %ERRORLEVEL%