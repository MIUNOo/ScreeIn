// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ScreeIn : ModuleRules
{
	public ScreeIn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AkAudio",
			"WwiseSoundEngine",
			"SlateCore"
		});
	}
}
