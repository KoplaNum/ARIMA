@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..") do set "REPO_ROOT=%%~fI"

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
if errorlevel 1 exit /b 1

cd /d C:\Users\OONN\Zorro
set "INCLUDE=%INCLUDE%;include"

cl /TC /Fo"Cache\\" /EHsc /fp:strict /Zc:wchar_t /Gd /MT /O2 /D "WIN32" /D "_WINDLL" /D "_MBCS" "%REPO_ROOT%\build\zorro32\autoari_zorro32_wrapper.c" /link /DLL /NOLOGO /DEF:"%REPO_ROOT%\build\zorro32\autoari_zorro32.def" /IMPLIB:"%REPO_ROOT%\build\zorro32\AutoAri32.lib" /OUT:"%REPO_ROOT%\build\zorro32\AutoAri32.dll" > Log\compiler.log 2>&1
exit /b %errorlevel%
