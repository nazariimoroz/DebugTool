// FireProject by likapu company


#include "include/DebugTool/DT_Logger.h"
#include "Kismet/KismetStringLibrary.h"
#include <inttypes.h>

UDT_Logger* UDT_Logger::Singleton = nullptr;

UDT_Logger::UDT_Logger()
{
    if(GEngine)
    {
        bUseDelegates = false;
        GEngine->OnWorldAdded().AddUObject(this, &ThisClass::OnWorldAddadCallback);
        GEngine->OnWorldDestroyed().AddUObject(this, &ThisClass::OnWorldDestroyedCallback);

        bInited = true;
    }
}

void UDT_Logger::WriteLine(const ELogVerbosity::Type LogVerbosity, const FString& FilePath, const uint64 Line, const FString& Str)
{
    if(!bInited)
    {
        UE_LOG(LogTemp, Error, TEXT("Logger is not inited"));
        return;
    }

    int32 BeginPos;
    int32 EndPos;

    if(!FilePath.FindLastChar('\\', BeginPos) && !FilePath.FindLastChar('/', BeginPos))
        BeginPos = 0;
    else
        BeginPos += 1;

    if(!FilePath.FindLastChar('.', EndPos))
    {
        UE_LOG(LogTemp, Error, TEXT("Bad FilePath"));
        return;
    }

    const auto FileName = UKismetStringLibrary::GetSubstring(FilePath, BeginPos, EndPos - BeginPos);

    const auto Final = FString::Printf(TEXT("%s(%" PRIu64 "): %s"), *FileName, Line, *Str);
    const auto FinalText = FText::FromString(Final);

    auto LogElement = FLogElement();
    LogElement.LogText = FinalText;
    LogElement.SetLogVerbosity(LogVerbosity);

    if(OnAddLogDelegate)
    {
        if(bUseDelegates) OnAddLogDelegate(LogVerbosity, Final);

        LoggerList.InsertNode(LogElement);
    }
    if(GEngine && GEngine->GetCurrentPlayWorld() && OnAddLogInGameDelegate)
    {
        OnAddLogInGameDelegate(LogVerbosity, Final);

        LoggerListInGame.InsertNode(LogElement);
    }
}

void UDT_Logger::Breakpoint(const FString& FilePath, const uint64 Line)
{
    WriteLineFormat(ELogVerbosity::Error, FilePath, Line, TEXT("BREAKPOINT"));
}

TArray<FLogElement> UDT_Logger::GetLastLogsInGame(int32 Count)
{
    int32 Added = 0;
    TArray<FLogElement> ToRet;
    for(const auto* Iter = LoggerListInGame.GetTail();
        Iter && Added < Count;
        Iter = Iter->GetPrevNode(), Added += 1)
    {
        ToRet.Add(Iter->GetValue());
    }
    return ToRet;
}

void UDT_Logger::OnWorldAddadCallback(UWorld* World)
{
    WorldCount += 1;
}

void UDT_Logger::OnWorldDestroyedCallback(UWorld* World)
{
    WorldCount -= 1;
}

