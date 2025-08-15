@echo off
echo Switching VITRUM AXIS to DX11 configuration...
echo.

REM Backup current configuration
if exist "Config\DefaultEngine.ini" (
    copy "Config\DefaultEngine.ini" "Config\DefaultEngine_DX12_Backup.ini"
    echo Backed up current DX12 configuration
)

REM Switch to DX11 configuration
copy "Config\DefaultEngine_DX11_Backup.ini" "Config\DefaultEngine.ini"
echo Switched to DX11 configuration

echo.
echo Configuration changed:
echo - TargetedRHIs: PCD3D_SM5 (DX11)
echo - Lumen: Disabled
echo - Nanite: Disabled
echo - Advanced features: Disabled for compatibility
echo.

echo You can now try launching the project again.
echo To revert to DX12, run: copy Config\DefaultEngine_DX12_Backup.ini Config\DefaultEngine.ini
echo.
pause

