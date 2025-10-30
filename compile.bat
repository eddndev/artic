@echo off
echo ================================
echo   Compilando Artic Compiler
echo ================================

echo.
echo Compilando ejecutable principal...
g++ -std=c++20 -Isrc ^
    src/frontend/lexer/Token.cpp ^
    src/frontend/lexer/Lexer.cpp ^
    src/frontend/parser/Parser.cpp ^
    src/backend/css/UtilityResolver.cpp ^
    src/backend/css/CssGenerator.cpp ^
    src/backend/html/HtmlGenerator.cpp ^
    src/compiler/ComponentRegistry.cpp ^
    src/compiler/Compiler.cpp ^
    src/shared/utils/FileSystem.cpp ^
    src/cli/main.cpp ^
    -o artic.exe

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Fallo al compilar artic.exe
    exit /b 1
)

echo.
echo ================================
echo   Compilacion completada!
echo ================================
echo.
echo artic.exe compilado exitosamente