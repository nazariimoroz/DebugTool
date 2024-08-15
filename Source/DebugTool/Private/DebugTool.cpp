// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugTool.h"
#include "DT_Settings.h"
#include "DebugTool/DT_Logger.h"
#include "DebugTool/DT_Observer.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolModule::StartupModule()
{
    if (UDT_Logger::Singleton)
    {
        UE_LOG(LogTemp, Error, TEXT("UDT_Logger::Singleton must be nullptr there"));
        return;
    }
    UDT_Logger::Singleton = new UDT_Logger();

    if (UDT_Observer::Singleton)
    {
        UE_LOG(LogTemp, Error, TEXT("UDT_Observer::Singleton must be nullptr there"));
        return;
    }
    UDT_Observer::Singleton = new UDT_Observer();

#if WITH_EDITOR
    // Custom Settings
    {
        if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            SettingsModule->RegisterSettings("Project", "Plugins", "DebugTool",
                LOCTEXT("RuntimeSettingsName", "Debug Tool"), LOCTEXT("RuntimeSettingsDescription", "Debug Tool"),
                GetMutableDefault<UDT_Settings>());
        }

    }
#endif
}

void FDebugToolModule::ShutdownModule()
{
    delete UDT_Logger::Singleton;
    UDT_Logger::Singleton = nullptr;
    delete UDT_Observer::Singleton;
    UDT_Observer::Singleton = nullptr;

#if WITH_EDITOR
    // Custom Settings
    {
        if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            SettingsModule->UnregisterSettings("Project", "Plugins", "DebugToolSettings");
        }
    }
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebugToolModule, DebugTool)
