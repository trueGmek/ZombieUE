// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class Zombie: ModuleRules {
  public Zombie(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] {
      "Core",
      "CoreUObject",
      "Engine",
      "InputCore",
      "EnhancedInput",
      "Combat",
      "GameplayTags"
    });

// Note. Use 'SetupGameplayDebuggerSupport(Target)' when adding module to your project's Build.cs (see AIModule/AIModule.Build.cs)
    SetupGameplayDebuggerSupport(Target);
  }
}
