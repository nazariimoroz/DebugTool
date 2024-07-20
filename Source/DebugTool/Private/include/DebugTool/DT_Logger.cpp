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

void UDT_Logger::WriteLine(const ELogVerbosity::Type LogVerbosity, const FString& FilePath, const uint64 Line, const FString& Str)
{
    if (!bInited)
    {
        UE_LOG(LogTemp, Error, TEXT("Logger is not inited"));
        return;
    }

    int32 BeginPos;
    int32 EndPos;

    if (!FilePath.FindLastChar('\\', BeginPos) && !FilePath.FindLastChar('/', BeginPos))
        BeginPos = 0;
    else
        BeginPos += 1;

    if (!FilePath.FindLastChar('.', EndPos))
    {
        UE_LOG(LogTemp, Error, TEXT("Bad FilePath"));
        return;
    }

    const auto FileName = UKismetStringLibrary::GetSubstring(FilePath, BeginPos, EndPos - BeginPos);

    const auto Final = FString::Printf(TEXT("%s(%" PRIu64 "): %s"), *FileName, Line, *Str);
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

void UDT_Logger::Breakpoint(const FString& FilePath, const uint64 Line)
{
    WriteLineFormat(ELogVerbosity::Error, FilePath, Line, TEXT("BREAKPOINT"));
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
