// FireProject by likapu company


#include "include/DebugTool/DT_Logger.h"

#include <chrono>
#include <inttypes.h>

UDT_ChainLogger::UDT_ChainLogger(const ELogVerbosity::Type InLogVerbosity, const std::string& InCategory, const uint64 InLine): LogVerbosity(InLogVerbosity)
    , Category(InCategory)
    , Line(InLine)
{}

UDT_ChainLogger::~UDT_ChainLogger()
{
    if(const auto Logger = UDT_Logger::Get())
        Logger->WriteLine(LogVerbosity, Category, Line, StringBuilder.ToString());
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const char* Value)
{
    StringBuilder.Append(Value);
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const wchar_t* Value)
{
    StringBuilder.Append(Value);
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const int32 Value)
{
    StringBuilder.Append(FString::FromInt(Value));
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const float Value)
{
    StringBuilder.Append(FString::SanitizeFloat(Value));
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const bool Value)
{
    StringBuilder.Append(Value ? "True" : "False");
    return *this;
}

UDT_Logger* UDT_Logger::Singleton = nullptr;

UDT_Logger::UDT_Logger()
{
    bUseDelegates = true;

    if(bUseLoggerFile)
    {
        time_t now = time(0);
        struct tm  tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        LoggerFile.open((std::stringstream() << "Log_" << buf << ".txt").str());
    }

    bInited = true;
}

UDT_Logger::~UDT_Logger()
{
    if(bUseLoggerFile)
        LoggerFile.close();
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

    if(bUseLoggerFile)
    {
        if(LoggerFile.is_open())
        {
            LoggerFile << GetData(Final);
        }
        else
        {
            DT_ERROR_NO_LOGGER("{0}", "LoggerFile is closed");
        }
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

UDT_ChainLogger UDT_Logger::CreateChainLogger(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line) const
{
    return UDT_ChainLogger(LogVerbosity, Category, Line);
}

