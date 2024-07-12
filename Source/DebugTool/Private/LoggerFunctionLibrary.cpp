// FireProject by likapu company


#include "LoggerFunctionLibrary.h"

#include "DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"
#include "Kismet/GameplayStatics.h"

TArray<UDT_LogElementInfo*> ULoggerFunctionLibrary::GetLastLogsInGame(int32 Count)
{
    const auto ToCopy = UDT_Logger::Get()->GetLastLogsInGame(Count);
    TArray<UDT_LogElementInfo*> ToRet;
    for (const auto& i : ToCopy)
    {
        const auto ToSet = NewObject<UDT_LogElementInfo>();
        ToSet->LogText = i.LogText;
        ToSet->LogVerbosityColor = i.LogVerbosityColor;
        ToRet.Add(ToSet);
    }

    return ToRet;
}
