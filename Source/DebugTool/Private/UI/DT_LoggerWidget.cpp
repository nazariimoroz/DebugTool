// FireProject by likapu company


#include "UI/DT_LoggerWidget.h"

#include "DebugTool/DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"
#include "UI/DT_ListView.h"

bool UDT_LoggerWidget::Refresh()
{
    if(const auto Log = UDT_Logger::Get())
    {
        LoggerView->ClearListItems();

        const auto LastLogsInGame = Log->GetLastLogsInGame(500);

        for (const auto& i : LastLogsInGame)
        {
            AddLog(i);
        }

        return true;
    }

    return false;
}

void UDT_LoggerWidget::AddLog(UDT_LogElementInfo* const LogElementInfo)
{
    DT_RETURN_NO_LOGGER(LoggerView);

    LoggerView->AddItemAt(LogElementInfo, 0);
}

void UDT_LoggerWidget::AddLog(const FDT_LogElement& LogElement)
{
    UDT_LogElementInfo* LogElementInfo = NewObject<UDT_LogElementInfo>();
    LogElementInfo->LogVerbosityColor = LogElement.LogVerbosityColor;
    LogElementInfo->LogText = LogElement.LogText;
    AddLog(LogElementInfo);
}
