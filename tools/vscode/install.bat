@echo off
echo Installing Artic Language Extension for VS Code...
echo.

set "EXTENSION_DIR=%USERPROFILE%\.vscode\extensions\artic-language"

if exist "%EXTENSION_DIR%" (
    echo Removing existing extension...
    rmdir /s /q "%EXTENSION_DIR%"
)

echo Creating extension directory...
mkdir "%EXTENSION_DIR%"

echo Copying files...
xcopy /s /i /y "%~dp0*" "%EXTENSION_DIR%"

echo.
echo Installation complete!
echo.
echo Please reload VS Code to activate the extension:
echo 1. Press Ctrl+Shift+P
echo 2. Type "Reload Window" and press Enter
echo.
echo Or simply restart VS Code.
echo.
pause
