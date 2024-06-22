// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugToolCommands.h"

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DebugTool", "Bring up DebugTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
