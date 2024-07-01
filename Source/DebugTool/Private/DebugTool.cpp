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

static const FName DebugToolTabName("DebugTool");

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolModule::StartupModule()
{
    AsyncTask(ENamedThreads::GameThread, []
    {
        if(UDT_Logger::Singleton)
        {
            UE_LOG(LogTemp, Error, TEXT("UDT_Logger::Singleton must be nullptr there"));
            return;
        }
        UDT_Logger::Singleton = NewObject<UDT_Logger>();
    });

	FDebugToolStyle::Initialize();
	FDebugToolStyle::ReloadTextures();

	FDebugToolCommands::Register();
	
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
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDebugToolStyle::Shutdown();

	FDebugToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DebugToolTabName);
}

TSharedRef<SDockTab> FDebugToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    UObject* Blueprint = UEditorAssetLibrary::LoadAsset(FString(TEXT("EditorUtilityWidgetBlueprint'/DebugTool/EUW_Logger.EUW_Logger'")));
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