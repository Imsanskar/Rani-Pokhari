@echo off

setlocal

if not exist "bin" mkdir bin
pushd bin

set compile_type="Debug"

if "%1"=="debug" goto DebugConfig
if "%1"=="release" goto ReleaseConfig

:DebugConfig
echo -------------------------------------
echo 	Debug Build configured
echo -------------------------------------


cmake -DCMAKE_BUILD_TYPE=Debug -B./ -G"Unix Makefiles" ../
make

goto DoneConfig

:ReleaseConfig
echo -------------------------------------
echo 	Release Build configured
echo -------------------------------------

cmake -DCMAKE_BUILD_TYPE=Release -B./  -G"Unix Makefiles" ../
make
goto DoneConfig

:DoneConfig
if not exist "assimp.exe" copy "..\Dependencies\lib\Debug\assimp.exe" "assimp.exe"
if not exist "zlib.dll" copy "..\Dependencies\lib\Debug\zlib.dll" "zlib.dll"
RaniPokhari.exe

@REM popd