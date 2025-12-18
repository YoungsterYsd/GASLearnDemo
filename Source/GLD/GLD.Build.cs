// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GLD : ModuleRules
{
	public GLD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",

            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",
            "Slate",
            "SlateCore",
            "HTTP",
            "JsonUtilities",
            "Json",
            "ModularGameplay",
            "UMG"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GLD",
            "GLD/AbilitySystem",
            "GLD/AbilitySystem/Abilities",
            "GLD/AbilitySystem/Attributes",
            "GLD/Character",
            "GLD/Component",
            "GLD/DamageActor",
            "GLD/Feedback",
            "GLD/Feedback/NumberPopComponent",
            "GLD/GameMode",
            "GLD/Item",
            "GLD/Player",
            "GLD/System",
            "GLD/UI",
            "GLD/UI/Common",
            "GLD/UI/Core",
            "GLD/UI/Damage",
            "GLD/UI/Equipment",
            "GLD/UI/Inventory",
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
