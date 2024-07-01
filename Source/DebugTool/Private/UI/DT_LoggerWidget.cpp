// FireProject by likapu company


#include "UI/DT_LoggerWidget.h"

#include "CommonListView.h"
#include "DebugTool/DT_Logger.h"
#include "UI/DT_LogElement.h"

bool UDT_LoggerWidget::Initialize()
{
    bool bToRet = Super::Initialize();

    AsyncTask(ENamedThreads::GameThread, [this]
    {
        if(const auto Logger = UDT_Logger::Get())
        {
            Logger->OnAddLogDelegate = [this](ELogVerbosity::Type LogVerbosity, FString String)
            {
                this->AddLog(LogVerbosity, FText::FromString(String));
            };
        }
    });

    return bToRet;
}

void UDT_LoggerWidget::AddLog(UDT_LogElementInfo* const LogElementInfo)
{
    LoggerView->AddItem(LogElementInfo);
}

void UDT_LoggerWidget::AddLog(const ELogVerbosity::Type LogVerbosity, const FText& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("WORK???"));
    UDT_LogElementInfo* LogElementInfo = NewObject<UDT_LogElementInfo>();
    LogElementInfo->LogVerbosity = LogVerbosity;
    LogElementInfo->LogText = Message;
    LoggerView->AddItem(LogElementInfo);
}

void UDT_LoggerWidget::AddLogCallback(ELogVerbosity::Type LogVerbosity, FString Message)
{
    AddLog(LogVerbosity, FText::FromString(Message));
}
