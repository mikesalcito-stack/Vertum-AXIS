@echo off
echo VITRUM AXIS - RHI Platform Fix
echo.

echo Step 1: Cleaning project files...
if exist "Intermediate" rmdir /s /q "Intermediate"
if exist "Saved" rmdir /s /q "Saved"
if exist "Binaries" rmdir /s /q "Binaries"
echo Cleaned project directories.

echo.
echo Step 2: Regenerating project files...
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="C:\VITRUM_AXIS\VITRUM_AXIS.uproject" -game -rocket -progress
echo Project files regenerated.

echo.
echo Step 3: Creating minimal engine configuration...
echo [/Script/WindowsTargetPlatform.WindowsTargetSettings] > Config\DefaultEngine.ini
echo TargetedRHIs=(PCD3D_SM5) >> Config\DefaultEngine.ini
echo. >> Config\DefaultEngine.ini
echo [/Script/Engine.Engine] >> Config\DefaultEngine.ini
echo r.GraphicsAdapter=0 >> Config\DefaultEngine.ini
echo r.RHICmdBypass=1 >> Config\DefaultEngine.ini
echo r.RHICmdWidth=0 >> Config\DefaultEngine.ini
echo r.RHICmdUseDeferredContexts=0 >> Config\DefaultEngine.ini
echo r.RHICmdUseThread=0 >> Config\DefaultEngine.ini
echo r.RHIThread.Enable=0 >> Config\DefaultEngine.ini
echo r.RHIThread.EnableTasks=0 >> Config\DefaultEngine.ini
echo r.DynamicGlobalIlluminationMethod=0 >> Config\DefaultEngine.ini
echo r.ReflectionMethod=0 >> Config\DefaultEngine.ini
echo r.Shaders.Optimize=0 >> Config\DefaultEngine.ini
echo ActiveGameNameRedirects=(OldName="/Script/TP_VehicleAdv",NewName="/Script/VITRUMAXIS") >> Config\DefaultEngine.ini
echo ActiveClassRedirects=(OldClassName="TP_VehicleAdvPawn",NewClassName="VitrumMarble") >> Config\DefaultEngine.ini
echo. >> Config\DefaultEngine.ini
echo [/Script/Engine.RendererSettings] >> Config\DefaultEngine.ini
echo r.SkinCache=0 >> Config\DefaultEngine.ini
echo r.ShadowQuality=0 >> Config\DefaultEngine.ini
echo r.ReflectionMethod=0 >> Config\DefaultEngine.ini
echo r.GlobalIlluminationMethod=0 >> Config\DefaultEngine.ini
echo r.DynamicGlobalIlluminationMethod=0 >> Config\DefaultEngine.ini
echo r.VirtualTextures=False >> Config\DefaultEngine.ini
echo r.VirtualTexturedLightmaps=False >> Config\DefaultEngine.ini
echo r.AllowOcclusionQueries=False >> Config\DefaultEngine.ini
echo r.AmbientOcclusionLevels=0 >> Config\DefaultEngine.ini
echo r.DepthOfFieldQuality=0 >> Config\DefaultEngine.ini
echo r.MotionBlurQuality=0 >> Config\DefaultEngine.ini
echo r.AntiAliasingMethod=0 >> Config\DefaultEngine.ini
echo r.MSAACount=0 >> Config\DefaultEngine.ini
echo r.SSR=0 >> Config\DefaultEngine.ini
echo r.SSS=0 >> Config\DefaultEngine.ini
echo r.LandscapeLOD=0 >> Config\DefaultEngine.ini
echo r.Tessellation=0 >> Config\DefaultEngine.ini
echo r.Nanite=False >> Config\DefaultEngine.ini
echo. >> Config\DefaultEngine.ini
echo [/Script/Engine.AudioSettings] >> Config\DefaultEngine.ini
echo bStreamAudioCache=False >> Config\DefaultEngine.ini
echo AudioSampleRate=22050 >> Config\DefaultEngine.ini
echo AudioNumSourceWorkers=1 >> Config\DefaultEngine.ini
echo. >> Config\DefaultEngine.ini
echo [/Script/Engine.GameUserSettings] >> Config\DefaultEngine.ini
echo bUseVSync=False >> Config\DefaultEngine.ini
echo ResolutionSizeX=1280 >> Config\DefaultEngine.ini
echo ResolutionSizeY=720 >> Config\DefaultEngine.ini
echo FullscreenMode=0 >> Config\DefaultEngine.ini
echo bUseDynamicResolution=False >> Config\DefaultEngine.ini
echo Minimal configuration created.

echo.
echo Step 4: Testing project launch...
echo Launching with minimal configuration...
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "C:\VITRUM_AXIS\VITRUM_AXIS.uproject" -nullrhi

echo.
echo If the project still crashes, try:
echo 1. Reinstall Unreal Engine 5.6
echo 2. Check Windows graphics drivers
echo 3. Run as administrator
echo.
pause

