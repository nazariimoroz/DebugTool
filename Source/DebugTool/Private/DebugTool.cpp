// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugTool.h"
#include "DebugToolStyle.h"
#include "DebugToolCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "DebugTool/DT_Logger.h"

static const FName DebugToolTabName("DebugTool");

#define LOCTEXT_NAMESPACE "FDebugToolModule"

void FDebugToolModule::StartupModule()
{
    if(UDT_Logger::Singleton)
    {
        UE_LOG(LogTemp, Error, TEXT("UDT_Logger::Singleton must be nullptr there"));
        return;
    }
    UDT_Logger::Singleton = NewObject<UDT_Logger>();

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
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FDebugToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("DebugTool.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
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