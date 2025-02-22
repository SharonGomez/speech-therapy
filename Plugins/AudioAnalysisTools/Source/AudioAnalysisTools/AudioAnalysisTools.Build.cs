// Georgy Treshchev 2023.

using UnrealBuildTool;

public class AudioAnalysisTools : ModuleRules
{
	public AudioAnalysisTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Core"
			}
		);

		PrivateDependencyModuleNames.Add("RuntimeAudioImporter");
	}
}