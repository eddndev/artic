@echo off
REM Artic Build Script for Windows

echo Building Artic Compiler...

REM Create build directory
if not exist build mkdir build

REM Configure CMake
echo Configuring CMake...
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DARTIC_BUILD_TESTS=ON

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    exit /b %ERRORLEVEL%
)

REM Build
echo Building...
cmake --build build --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo.
echo Build completed successfully!
echo Binary location: build\Release\artic.exe
echo Tests location: build\Release\artic_tests.exe
echo.
echo Run tests: build\Release\artic_tests.exe
echo Try lexer: build\Release\artic.exe lex examples\hello_world\index.atc
