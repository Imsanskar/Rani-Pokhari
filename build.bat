@echo off

setlocal

if not exist "build" mkdir build
pushd build

set compile_type="Debug"

if "%1"=="debug" goto DebugConfig
if "%1"=="release" goto ReleaseConfig

:DebugConfig
echo -------------------------------------
echo 	Debug Build configured
echo -------------------------------------


cmake -DCMAKE_BUILD_TYPE=Debug -B./ -G"Unix Makefiles" ../
make

if %ERRORLEVEL% NEQ 0 goto CompileError
goto Run


:ReleaseConfig
echo -------------------------------------
echo 	Release Build configured
echo -------------------------------------

cmake -DCMAKE_BUILD_TYPE=Release -B./  -G"Unix Makefiles" ../
make
if %ERRORLEVEL% NEQ 0 goto CompileError
goto Run

:Run
if not exist "assimp.exe" copy "..\Dependencies\lib\Debug\assimp.exe" "assimp.exe"
if not exist "zlib.dll" copy "..\Dependencies\lib\Debug\zlib.dll" "zlib.dll"
RaniPokhari.exe
goto Done

:CompileError
echo Compilation Failed

:Done
popd