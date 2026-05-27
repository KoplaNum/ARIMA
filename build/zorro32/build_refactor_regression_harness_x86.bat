@echo off
setlocal

set "SCRIPT_DIR=%~dp0"

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
if errorlevel 1 exit /b 1

cl /TC /O2 /MD /Fe"%SCRIPT_DIR%refactor_regression_harness.exe" "%SCRIPT_DIR%refactor_regression_harness.c" /link /NOLOGO /LIBPATH:"%SCRIPT_DIR%" AutoAri32.lib
exit /b %errorlevel%
