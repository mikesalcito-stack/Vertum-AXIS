@echo off
echo ========================================
echo VITRUM AXIS - DirectX 12 Compatibility Test
echo ========================================
echo.

echo [1/5] Cleaning previous build files...
if exist "Intermediate" rmdir /s /q "Intermediate"
if exist "Saved\Logs" rmdir /s /q "Saved\Logs"
if exist "Saved\Crashes" rmdir /s /q "Saved\Crashes"
echo ✓ Cleaned build files
echo.

echo [2/5] Regenerating project files...
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="C:\VITRUM_AXIS\VITRUM_AXIS.uproject" -game -rocket -progress
if %ERRORLEVEL% NEQ 0 (
    echo ✗ Failed to regenerate project files
    pause
    exit /b 1
)
echo ✓ Project files regenerated
echo.

echo [3/5] Building project...
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" VITRUM_AXIS Win64 Development -Project="C:\VITRUM_AXIS\VITRUM_AXIS.uproject" -WaitMutex -FromMsBuild
if %ERRORLEVEL% NEQ 0 (
    echo ✗ Build failed
    pause
    exit /b 1
)
echo ✓ Build completed successfully
echo.

echo [4/5] Checking DirectX 12 configuration...
findstr /C:"PCD3D_SM6" "Config\DefaultEngine.ini" >nul
if %ERRORLEVEL% EQU 0 (
    echo ✓ DirectX 12 target confirmed (PCD3D_SM6)
) else (
    echo ✗ DirectX 12 target not found
)

findstr /C:"D3D12RHI" "Source\VITRUMAXIS\VITRUMAXIS.Build.cs" >nul
if %ERRORLEVEL% EQU 0 (
    echo ✓ D3D12RHI module included
) else (
    echo ✗ D3D12RHI module missing
)
echo.

echo [5/5] Launching Unreal Editor...
echo Starting VITRUM_AXIS.uproject...
start "" "VITRUM_AXIS.uproject"
echo ✓ Editor launched
echo.

echo ========================================
echo Testing Complete!
echo ========================================
echo.
echo Please check the editor for:
echo - DirectX 12 initialization messages
echo - GPU selection (should show RTX 3060)
echo - No RHI platform validation errors
echo - Lumen and advanced features working
echo.
echo If issues persist, check the logs in:
echo Saved\Logs\VITRUM_AXIS.log
echo.
pause
