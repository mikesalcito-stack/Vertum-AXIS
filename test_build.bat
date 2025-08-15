@echo off
echo Testing VITRUM AXIS build...
echo.

REM Set timeout for build process
timeout /t 30 /nobreak > nul

REM Try to build the project
echo Starting build process...
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" VITRUM_AXIS Win64 Development -Project="C:\VITRUM_AXIS\VITRUM_AXIS.uproject" -WaitMutex -FromMsBuild

echo.
echo Build test completed.
pause

