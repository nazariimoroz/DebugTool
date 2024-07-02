// FireProject by likapu company


#include "UI/DT_LoggerWidget.h"

#include "CommonListView.h"
#include "DebugTool/DT_Logger.h"
#include "UI/DT_LogElement.h"

bool UDT_LoggerWidget::Initialize()
{
    bool bToRet = Super::Initialize();

    if(const auto Logger = UDT_Logger::Get())
    {
        Logger->OnAddLogDelegate = [this](ELogVerbosity::Type LogVerbosity, FString String)
        {
            this->AddLog(LogVerbosity, FText::FromString(String));
        };
    }
    else
        bToRet = false;

    return bToRet;
}

void UDT_LoggerWidget::BeginDestroy()
{
    if(const auto Logger = UDT_Logger::Get())
    {
        Logger->OnAddLogDelegate = nullptr;
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Cant get Logger on Destroying"));

    Super::BeginDestroy();
}

void UDT_LoggerWidget::AddLog(UDT_LogElementInfo* const LogElementInfo)
{
    LoggerView->AddItem(LogElementInfo);
}

void UDT_LoggerWidget::AddLog(const ELogVerbosity::Type LogVerbosity, const FText& Message)
{
    UDT_LogElementInfo* LogElementInfo = NewObject<UDT_LogElementInfo>();
    LogElementInfo->LogVerbosity = LogVerbosity;
    LogElementInfo->LogText = Message;
    AddLog(LogElementInfo);
}

void UDT_LoggerWidget::AddLogCallback(ELogVerbosity::Type LogVerbosity, FString Message)
{
    AddLog(LogVerbosity, FText::FromString(Message));
}
