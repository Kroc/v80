@REM # this batch file makes it easier to call v80 as part of other batch files.
@REM # CP/M doesn't support directories so we must always invoke NTVCM+V80 from
@REM # the current working directory -- if it isn't already, we change the
@REM # directory to the same one as this script
@PUSHD %~dp0
@ntvcm.exe v80.com %*
@POPD & EXIT /B %ERRORLEVEL%