// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugTool.h"
#include "DebugToolStyle.h"
#include "DebugToolCommands.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "DebugTool/DT_Logger.h"
#include "DebugTool/DT_Observer.h"

static const FName DebugToolTabName("DebugTool");

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolModule::OnEditorInitializedCallback(double X)
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
}

void FDebugToolModule::StartupModule()
{
    FDebugToolStyle::Initialize();
    FDebugToolStyle::ReloadTextures();

    FDebugToolCommands::Register();

#if WITH_EDITOR
    FEditorDelegates::OnEditorInitialized.AddRaw(this, &FDebugToolModule::OnEditorInitializedCallback);
#else
    DT_ERROR_NO_LOGGER("{0}", "TODO");
#endif

    PluginCommands = MakeShareable(new FUICommandList);

    PluginCommands->MapAction(
        FDebugToolCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateRaw(this, &FDebugToolModule::PluginButtonClicked),
        FCanExecuteAction());

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDebugToolModule::RegisterMenus));

    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DebugToolTabName, FOnSpawnTab::CreateRaw(this, &FDebugToolModule::OnSpawnPluginTab))
                            .SetDisplayName(LOCTEXT("FDebugToolTabTitle", "DebugTool"))
                            .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FDebugToolModule::ShutdownModule()
{
    delete UDT_Logger::Singleton;
    UDT_Logger::Singleton = nullptr;
    delete UDT_Observer::Singleton;
    UDT_Observer::Singleton = nullptr;

    UToolMenus::UnRegisterStartupCallback(this);

    UToolMenus::UnregisterOwner(this);

    FDebugToolStyle::Shutdown();

    FDebugToolCommands::Unregister();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DebugToolTabName);
}

TSharedRef<SDockTab> FDebugToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    UObject* Blueprint = UEditorAssetLibrary::LoadAsset(FString(TEXT("EditorUtilityWidgetBlueprint'/DebugTool/UI/InEditor/EUW_InEditorLogger.EUW_InEditorLogger'")));
    if (Blueprint)
    {
        UEditorUtilityWidgetBlueprint* EditorWidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
        if (EditorWidgetBlueprint)
        {
            UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
            if (EditorUtilitySubsystem)
            {
                FName ID;
                UEditorUtilityWidget* EditorWidget = EditorUtilitySubsystem->SpawnAndRegisterTabAndGetID(EditorWidgetBlueprint, ID);

                if (EditorWidget)
                {
                    auto DockTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)[EditorWidget->TakeWidget()];
                    EditorUtilitySubsystem->CloseTabByID(ID);
                    return DockTab;
                }
            }
        }
    }
    return SNew(SDockTab);
}

void FDebugToolModule::PluginButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(DebugToolTabName);
}

void FDebugToolModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    {
        UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.SettingsToolbar");
        {
            FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Debug");
            {
                FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FDebugToolCommands::Get().OpenPluginWindow));
                Entry.SetCommandList(PluginCommands);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebugToolModule, DebugTool)
