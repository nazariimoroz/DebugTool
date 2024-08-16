// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugToolEditorCommands.h"

#define LOCTEXT_NAMESPACE "FDebugToolEditorModule"

void FDebugToolEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DebugToolEditor", "Bring up DebugToolEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
