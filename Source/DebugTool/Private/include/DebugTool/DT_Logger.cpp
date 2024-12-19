// FireProject by likapu company


#include "include/DebugTool/DT_Logger.h"

#include <chrono>
#include <inttypes.h>

#include "DT_Settings.h"

UDT_ChainLogger::UDT_ChainLogger(const ELogVerbosity::Type InLogVerbosity, FString&& InCategory, const uint64 InLine)
    : LogVerbosity(InLogVerbosity)
    , Category(InCategory)
    , Line(InLine)
{}

UDT_ChainLogger::~UDT_ChainLogger()
{
    if(const auto Logger = UDT_Logger::Get())
        Logger->WriteLine(LogVerbosity, MoveTempIfPossible(Category), Line, StringBuilder.ToString());
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

UDT_ChainLogger& UDT_ChainLogger::operator<<(const UObject* const Value)
{
    StringBuilder.Append(GetData(Value->GetName()));
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const FString& Value)
{
    StringBuilder.Append(Value);
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const FName& Value)
{
    StringBuilder.Append(Value.ToString());
    return *this;
}

UDT_ChainLogger& UDT_ChainLogger::operator<<(const FText& Value)
{
    StringBuilder.Append(Value.ToString());
    return *this;
}

UDT_Logger* UDT_Logger::Singleton = nullptr;

UDT_Logger::UDT_Logger()
{
    bUseDelegates = true;
    bUseLoggerFile = true;

#if WITH_EDITOR
    ReloadLogFileFromSettingsClass();
#else
    /*
    if(bUseLoggerFile)
    {
        FString FileDir = FPaths::AutomationLogDir();
        const auto FileName = FPaths::Combine(FileDir, FString::Printf(TEXT("Log_%hs.txt"), buf));
        LoggerFile.open(*FileName, std::ios::out);
    }*/
#endif

    bInited = true;
}

UDT_Logger::~UDT_Logger()
{
    if(bUseLoggerFile)
        LoggerFile.close();
}

void UDT_Logger::WriteLine(const ELogVerbosity::Type LogVerbosity, FString&& Category, const uint64 Line, FString&& Str)
{
    if (!bInited)
    {
        UE_LOG(LogTemp, Error, TEXT("Logger is not inited"));
        return;
    }

    auto LogElement = FDT_LogElement();
    LogElement.LogText = MoveTempIfPossible(Str);
    LogElement.Category = MoveTempIfPossible(Category);
    LogElement.Line = Line;
    LogElement.LogVerbosity = LogVerbosity;
    if (LogVerbosity == ELogVerbosity::Error)
    {
        LogElement.StackTrace = DT_GET_STACKTRACE();
    }

    if (bUseDelegates) OnAddLogDelegate.Broadcast(LogElement);
    LoggerList.AddTail(LogElement);

    /*
    if(bUseLoggerFile)
    {
        if(LoggerFile.is_open())
        {
            // LoggerFile << GetData(Final) << std::endl;
        }
        else
        {
            // DT_ERROR_NO_LOGGER("{0}", "LoggerFile is closed");
        }
    }*/
}

void UDT_Logger::Breakpoint(FString&& Category, const uint64 Line)
{
    WriteLineFormat(ELogVerbosity::Error, MoveTempIfPossible(Category), Line, TEXT("BREAKPOINT"));
}

TArray<FDT_LogElement> UDT_Logger::GetLastLogs(int32 Count) const
{
    int32 Added = 0;
    TArray<FDT_LogElement> ToRet;
    for (auto Iter = LoggerList.GetTail();
         Iter && (Count == -1 || Added < Count);
         Iter = Iter->GetPrevNode(), Added += 1)
    {
        ToRet.Insert(Iter->GetValue(), 0);
    }
    return ToRet;
}

UDT_ChainLogger UDT_Logger::CreateChainLogger(const ELogVerbosity::Type LogVerbosity, FString&& Category, const uint64 Line) const
{
    return UDT_ChainLogger(LogVerbosity, MoveTempIfPossible(Category), Line);
}

void UDT_Logger::ReloadLogFileFromSettingsClass()
{
    if(const auto Settings = GetMutableDefault<UDT_Settings>())
    {
        if(LoggerFile.is_open())
            LoggerFile.close();

        bUseLoggerFile = Settings->bEnableLogFile;
        if(bUseLoggerFile)
        {
            time_t now = time(0);
            struct tm  tstruct;
            char buf[80];
            tstruct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", &tstruct);

            FString FileDir;
            if(!Settings->LogFilePath.IsEmpty())
            {
                FileDir = Settings->LogFilePath;
            }
            else
            {
                FileDir = FPaths::ProjectLogDir();
            }
            const auto FileName = FPaths::Combine(FileDir, FString::Printf(TEXT("Log_%hs.txt"), buf));
            LoggerFile.open(*FileName, std::ios::out);
        }
    }
}

