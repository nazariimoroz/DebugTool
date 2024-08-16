// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugToolEditor.h"
#include "DebugToolEditorStyle.h"
#include "DebugToolEditorCommands.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Widgets/Docking/SDockTab.h"
#include "EditorUtilitySubsystem.h"
#include "ToolMenus.h"

static const FName DebugToolEditorTabName("DebugToolEditor");

#define LOCTEXT_NAMESPACE "FDebugToolEditorModule"

void FDebugToolEditorModule::StartupModule()
{
	FDebugToolEditorStyle::Initialize();
	FDebugToolEditorStyle::ReloadTextures();

	FDebugToolEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDebugToolEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FDebugToolEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDebugToolEditorModule::RegisterMenus));

    /*
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DebugToolEditorTabName, FOnSpawnTab::CreateRaw(this, &FDebugToolEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FDebugToolEditorTabTitle", "DebugToolEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);*/

    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DebugToolEditorTabName, FOnSpawnTab::CreateRaw(this, &FDebugToolEditorModule::OnSpawnPluginTab))
                        .SetDisplayName(LOCTEXT("FDebugToolTabTitle", "DebugTool"))
                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FDebugToolEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDebugToolEditorStyle::Shutdown();

	FDebugToolEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DebugToolEditorTabName);
}

TSharedRef<SDockTab> FDebugToolEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    UObject* Blueprint = UEditorAssetLibrary::LoadAsset(FString(TEXT("/DebugTool/UI/InEditor/WB_InEditorLogger")));
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

void FDebugToolEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(DebugToolEditorTabName);
}

void FDebugToolEditorModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    {
        UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.SettingsToolbar");
        {
            FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Debug");
            {
                FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FDebugToolEditorCommands::Get().OpenPluginWindow));
                Entry.SetCommandList(PluginCommands);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugToolEditorModule, DebugToolEditor)