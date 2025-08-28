// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugTool.h"
#include "DT_Settings.h"
#include "DebugTool/DT_LoggerSubsystem.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolModule::StartupModule()
{
}

void FDebugToolModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebugToolModule, DebugTool)
