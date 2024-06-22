// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DebugToolStyle.h"

class FDebugToolCommands : public TCommands<FDebugToolCommands>
{
public:

	FDebugToolCommands()
		: TCommands<FDebugToolCommands>(TEXT("DebugTool"), NSLOCTEXT("Contexts", "DebugTool", "DebugTool Plugin"), NAME_None, FDebugToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};