using UnrealBuildTool;

public class VITRUM_AXISTarget : TargetRules
{
	public VITRUM_AXISTarget( TargetInfo Target ) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "VITRUMAXIS" } );
	}
}
