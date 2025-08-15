@echo off
echo Building VITRUM AXIS Project...
echo.

REM Check if Unreal Engine is installed
if not exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    echo Error: Unreal Engine 5.3 not found at expected location.
    echo Please install Unreal Engine 5.3 or update the path in this script.
    pause
    exit /b 1
)

REM Set Unreal Engine path
set UE_PATH=C:\Program Files\Epic Games\UE_5.6

REM Build the project
echo Building VITRUM_AXIS module...
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" VITRUM_AXIS Win64 Development -Project="%CD%\VITRUM_AXIS.uproject" -WaitMutex -FromMsBuild

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build completed successfully!
    echo.
    echo To open the project in Unreal Engine:
    echo 1. Launch Unreal Engine 5.3
    echo 2. Open project: %CD%\VITRUM_AXIS.uproject
    echo.
) else (
    echo.
    echo Build failed with error code: %ERRORLEVEL%
    echo Please check the build output for details.
    echo.
)

pause
