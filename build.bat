@echo off

setlocal

if not exist "build" mkdir build
pushd build

@REM del CMakeCache.txt

set compile_type="debug"

if "%1"=="debug" goto DebugConfig
if "%1"=="release" goto ReleaseConfig

:DebugConfig
echo -------------------------------------
echo 	Debug Build configured
echo -------------------------------------

@REM where cl >nul 2>nul
@REM if %ERRORLEVEL% neq 0 goto SkipMSVC
@REM msbuild ../RaniPokhari.sln
@REM if %ERRORLEVEL% NEQ 0 goto CompileError
@REM goto Run


:SkipMSVC
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
if not exist "assimp-vc142-mtd.dll" copy "..\Dependencies\lib\Debug\assimp-vc142-mtd.dll" "assimp-vc142-mtd.dll"
if not exist "zlib.dll" copy "..\Dependencies\lib\Debug\zlib.dll" "zlib.dll"
RaniPokhari.exe
goto Done

:CompileError
echo Compilation Failed

:Done
popd