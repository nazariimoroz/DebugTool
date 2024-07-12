// FireProject by likapu company


#include "DebugTool/DT_LogElementInfo.h"

#include "DebugTool/DT_Logger.h"

UDT_LogElementInfo* UDT_LogElementInfo::Create(const FDT_LogElement& LogElement)
{
    const auto ToRet = NewObject<UDT_LogElementInfo>();
    ToRet->LogText = LogElement.LogText;
    ToRet->LogVerbosityColor = LogElement.LogVerbosityColor;
    return ToRet;
}
