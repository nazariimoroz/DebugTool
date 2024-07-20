// FireProject by likapu company


#include "UI/DT_LoggerWidget.h"

#include "CommonListView.h"
#include "DebugTool/DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"
#include "UI/DT_LogElementWidget.h"

bool UDT_LoggerWidget::Initialize()
{
    bool bToRet = Super::Initialize();

    if(const auto Logger = UDT_Logger::Get())
    {
        DT_ERROR_NO_LOGGER("{0}", "BAN");
    }
    else
        bToRet = false;

    return bToRet;
}

void UDT_LoggerWidget::BeginDestroy()
{
    Super::BeginDestroy();
}

void UDT_LoggerWidget::AddLog(UDT_LogElementInfo* const LogElementInfo)
{
    LoggerView->AddItem(LogElementInfo);
}

void UDT_LoggerWidget::AddLog(const FDT_LogElement& LogElement)
{
    UDT_LogElementInfo* LogElementInfo = NewObject<UDT_LogElementInfo>();
    LogElementInfo->LogVerbosityColor = LogElement.LogVerbosityColor;
    LogElementInfo->LogText = LogElement.LogText;
    AddLog(LogElementInfo);
}
