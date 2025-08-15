@echo off
echo VITRUM AXIS - Engine Diagnostic
echo.

echo Checking Unreal Engine 5.6 installation...
echo.

echo 1. Engine path check:
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" (
    echo [OK] UnrealEditor.exe found
) else (
    echo [ERROR] UnrealEditor.exe not found
)

echo.
echo 2. Build tool check:
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    echo [OK] UnrealBuildTool.exe found
) else (
    echo [ERROR] UnrealBuildTool.exe not found
)

echo.
echo 3. RHI modules check:
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\D3D11RHI.dll" (
    echo [OK] D3D11RHI.dll found
) else (
    echo [WARNING] D3D11RHI.dll not found
)

if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\D3D12RHI.dll" (
    echo [OK] D3D12RHI.dll found
) else (
    echo [WARNING] D3D12RHI.dll not found
)

echo.
echo 4. Graphics driver check:
dxdiag /t dxdiag_output.txt
timeout /t 5 /nobreak > nul
if exist dxdiag_output.txt (
    echo [OK] dxdiag output created
    echo Checking for DirectX version...
    findstr /i "DirectX Version" dxdiag_output.txt
) else (
    echo [ERROR] Could not create dxdiag output
)

echo.
echo 5. Project file check:
if exist "VITRUM_AXIS.uproject" (
    echo [OK] Project file found
    echo Project contents:
    type "VITRUM_AXIS.uproject"
) else (
    echo [ERROR] Project file not found
)

echo.
echo 6. Configuration check:
if exist "Config\DefaultEngine.ini" (
    echo [OK] DefaultEngine.ini found
    echo Current RHI settings:
    findstr /i "TargetedRHIs" "Config\DefaultEngine.ini"
) else (
    echo [ERROR] DefaultEngine.ini not found
)

echo.
echo 7. Windows version check:
ver
echo.

echo 8. Available memory:
wmic computersystem get TotalPhysicalMemory /value
echo.

echo Diagnostic complete. Check the output above for issues.
echo.
echo If RHI modules are missing, try:
echo 1. Repair Unreal Engine 5.6 installation
echo 2. Update graphics drivers
echo 3. Install Visual Studio 2022 with C++ tools
echo.
pause

