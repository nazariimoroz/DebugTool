// FireProject by likapu company


#include "UI/DT_LoggerInGameWidget.h"

#include "CommonListView.h"
#include "DebugTool/DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"
#include "DebugTool/DT_Observer.h"

bool UDT_LoggerInGameWidget::Initialize()
{
    bool bToRet = Super::Initialize();
    State = EDT_LoggerInGameWidgetState::None;

    if(const auto Logger = UDT_Logger::Get())
    {
        Logger->OnAddLogInGameDelegate.AddUObject(this, &ThisClass::OnAddLogInGameCallback);
    }
    else
        bToRet = false;

    if(const auto Observer = UDT_Observer::Get())
    {
        Observer->OnVariableChangedDelegate.AddUObject(this, &ThisClass::OnVariableChangedCallback);
    }
    else
        bToRet = false;

    return bToRet;
}

void UDT_LoggerInGameWidget::BeginDestroy()
{
    State = EDT_LoggerInGameWidgetState::None;

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

void UDT_LoggerInGameWidget::OnAddLogInGameCallback(FDT_LogElement LogElement)
{
    AddLog(LogElement);
}

void UDT_LoggerInGameWidget::OnVariableChangedCallback(TWeakPtr<FDT_VariableInfo> VariableInfo)
{
    if(const auto VarInfo = VariableInfo.Pin())
    {
        DT_DISPLAY("{0}: {1}", VarInfo->Property->GetName(), VarInfo->Value.Get<int32>());
    }
}
