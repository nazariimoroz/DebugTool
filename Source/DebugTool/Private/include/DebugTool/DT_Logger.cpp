// FireProject by likapu company


#include "include/DebugTool/DT_Logger.h"
#include "Kismet/KismetStringLibrary.h"
#include <inttypes.h>

UDT_Logger* UDT_Logger::Singleton = nullptr;

UDT_Logger::UDT_Logger()
{
}

void UDT_Logger::WriteLine(const ELogVerbosity::Type LogVerbosity, const FString& FilePath, const uint64 Line, const FString& Str)
{
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

    // TODO
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Final);
}

void UDT_Logger::Breakpoint(const FString& FilePath, const uint64 Line)
{
    WriteLineFormat(ELogVerbosity::Error, FilePath, Line, TEXT("BREAKPOINT"));
}

