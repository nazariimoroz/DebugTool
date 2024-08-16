// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DebugToolEditorStyle.h"

class FDebugToolEditorCommands : public TCommands<FDebugToolEditorCommands>
{
public:

	FDebugToolEditorCommands()
		: TCommands<FDebugToolEditorCommands>(TEXT("DebugToolEditor"), NSLOCTEXT("Contexts", "DebugToolEditor", "DebugToolEditor Plugin"), NAME_None, FDebugToolEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};