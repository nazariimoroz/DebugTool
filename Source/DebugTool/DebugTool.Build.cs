// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DebugTool : ModuleRules
{
	public DebugTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(PluginDirectory,"Source/DebugTool/Public/include/")
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				//"EditorFramework",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				//"Blutility",
				"UMG",
				"CommonUI"
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
