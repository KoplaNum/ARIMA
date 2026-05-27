@echo off
setlocal

set "SCRIPT_DIR=%~dp0"

if not exist "%SCRIPT_DIR%obj" mkdir "%SCRIPT_DIR%obj"

:: IMPORTANT: Update this path to match your Visual Studio installation
:: Example paths:
::   Visual Studio 2022 BuildTools: C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat
::   Visual Studio 2022 Community:  C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
::   Visual Studio 2019 BuildTools: C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat
::
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b 1

cl /TP /EHsc /O2 /MD /D "WIN64" /D "_WINDLL" /D "_MBCS" /Fo"%SCRIPT_DIR%obj\\" /Fe"%SCRIPT_DIR%AutoAri64.dll" "%SCRIPT_DIR%autoari_zorro64_wrapper.cpp" /link /DLL /NOLOGO /DEF:"%SCRIPT_DIR%autoari_zorro64.def" /IMPLIB:"%SCRIPT_DIR%AutoAri64.lib"
exit /b %errorlevel%
