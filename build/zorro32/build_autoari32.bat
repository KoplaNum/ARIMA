@echo off
setlocal

if "%TCC_EXE%"=="" set TCC_EXE=C:\tcc\tcc.exe

if not exist "%TCC_EXE%" (
  echo TCC compiler not found: %TCC_EXE%
  echo Set TCC_EXE to your 32-bit tcc.exe path and run again.
  exit /b 1
)

if not exist "..\..\bin\zorro32" mkdir "..\..\bin\zorro32"

"%TCC_EXE%" -shared -o "..\..\bin\zorro32\AutoAri32.dll" "autoari_zorro32_wrapper.c" "autoari_zorro32.def"
if errorlevel 1 (
  echo DLL build failed.
  exit /b 1
)

echo Built ..\..\bin\zorro32\AutoAri32.dll
exit /b 0
