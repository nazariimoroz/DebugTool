// FireProject by likapu company


#include "DebugTool/DT_LoggerFunctionLibrary.h"
#include "DebugTool/DT_LogElementInfo.h"
#include "DebugTool/DT_Logger.h"

TArray<UDT_LogElementInfo*> UDT_LoggerFunctionLibrary::GetLastLogsInGame(int32 Count)
{
    const auto ToCopy = UDT_Logger::Get()->GetLastLogsInGame(Count);
    TArray<UDT_LogElementInfo*> ToRet;
    for (const auto& i : ToCopy)
    {
        ToRet.Add(UDT_LogElementInfo::Create(i));
    }

    return ToRet;
}
