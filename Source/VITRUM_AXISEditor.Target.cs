using UnrealBuildTool;

public class VITRUM_AXISEditorTarget : TargetRules
{
	public VITRUM_AXISEditorTarget( TargetInfo Target ) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "VITRUMAXIS" } );
	}
}
