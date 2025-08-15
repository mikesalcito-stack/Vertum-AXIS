@echo off
echo VITRUM AXIS - DX12 RHI Fix Test
echo.

echo Current configuration:
echo - TargetedRHIs: PCD3D_SM6 (DX12) with PCD3D_SM5 fallback
echo - RHI Threading: Disabled for stability
echo - GPU: NVIDIA RTX 3060 (Primary)
echo.

echo Testing project launch...
echo.

REM Try to launch the project
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "C:\VITRUM_AXIS\VITRUM_AXIS.uproject"

echo.
echo If the project still crashes with RHI errors, try:
echo 1. Copy Config\DefaultEngine_DX11_Backup.ini to Config\DefaultEngine.ini
echo 2. Restart Unreal Editor
echo.
pause

