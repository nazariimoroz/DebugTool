// FireProject by likapu company


#include "UI/DT_LoggerInGameWidget.h"

#include "CommonListView.h"
#include "DebugTool/DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"

bool UDT_LoggerInGameWidget::Initialize()
{
    bool bToRet = Super::Initialize();
    State = EDT_LoggerInGameWidgetState::None;

    if(const auto Logger = UDT_Logger::Get())
    {
        Logger->OnAddLogInGameDelegate = [this](const FDT_LogElement& LogElement)
        {
            AddLog(LogElement);
        };
    }
    else
        bToRet = false;

    return bToRet;
}

void UDT_LoggerInGameWidget::BeginDestroy()
{
    State = EDT_LoggerInGameWidgetState::None;
    if(const auto Logger = UDT_Logger::Get())
    {
        Logger->OnAddLogDelegate = nullptr;
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Cant get Logger on Destroying"));

    Super::BeginDestroy();
}

void UDT_LoggerInGameWidget::AddLog(UDT_LogElementInfo* const LogElementInfo)
{
    DT_RETURN_NO_LOGGER(LoggerView);

    if(State == EDT_LoggerInGameWidgetState::Logger)
        LoggerView->AddItem(LogElementInfo);
}

void UDT_LoggerInGameWidget::AddLog(const FDT_LogElement& LogElement)
{
    UDT_LogElementInfo* LogElementInfo = NewObject<UDT_LogElementInfo>();
    DT_RETURN_NO_LOGGER(LogElementInfo);
    LogElementInfo->LogVerbosityColor = LogElement.LogVerbosityColor;
    LogElementInfo->LogText = LogElement.LogText;
    AddLog(LogElementInfo);
}
