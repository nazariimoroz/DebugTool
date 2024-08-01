// FireProject by likapu company


#include "include/DebugTool/DT_Logger.h"
#include "Kismet/KismetStringLibrary.h"
#include <inttypes.h>

UDT_Logger* UDT_Logger::Singleton = nullptr;

UDT_Logger::UDT_Logger()
{
    bUseDelegates = true;
    bInited = true;
}

void UDT_Logger::WriteLine(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line, const FString& Str)
{
    if (!bInited)
    {
        UE_LOG(LogTemp, Error, TEXT("Logger is not inited"));
        return;
    }
    const auto Final = FString::Printf(TEXT("%hs(%" PRIu64 "): %s"), Category.data(), Line, *Str);
    const auto FinalText = FText::FromString(Final);

    auto LogElement = FDT_LogElement();
    LogElement.LogText = FinalText;
    LogElement.SetLogVerbosity(LogVerbosity);

    if (bUseDelegates) OnAddLogDelegate.Broadcast(LogElement);
    LoggerList.AddTail(LogElement);

    if (GEngine && GEngine->GetCurrentPlayWorld())
    {
        if (bUseDelegates) OnAddLogInGameDelegate.Broadcast(LogElement);

        LoggerListInGame.AddTail(LogElement);
    }
}

void UDT_Logger::Breakpoint(const std::string& Category, const uint64 Line)
{
    WriteLineFormat(ELogVerbosity::Error, Category, Line, TEXT("BREAKPOINT"));
}

TArray<FDT_LogElement> UDT_Logger::GetLastLogsInGame(int32 Count) const
{
    int32 Added = 0;
    TArray<FDT_LogElement> ToRet;
    for (auto Iter = LoggerListInGame.GetTail();
         Iter && (Count == -1 || Added < Count);
         Iter = Iter->GetPrevNode(), Added += 1)
    {
        ToRet.Insert(Iter->GetValue(), 0);
    }
    return ToRet;
}
