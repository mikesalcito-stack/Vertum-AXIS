using UnrealBuildTool;

public class VITRUMAXIS : ModuleRules
{
	public VITRUMAXIS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"ModularGameplay",
			"PhysicsCore", "Chaos", "ChaosSolverEngine", "AudioMixer", "AudioExtensions",
			"Niagara", "UMG", "Slate", "SlateCore", "RenderCore", "RHI", "D3D12RHI",
			"AIModule", "NavigationSystem", "Renderer"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
