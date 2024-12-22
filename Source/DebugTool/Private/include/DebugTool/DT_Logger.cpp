// FireProject by likapu company

#include "include/DebugTool/DT_Logger.h"

#include <chrono>
#include <inttypes.h>

#include "DT_Settings.h"

UDT_ChainLogger::UDT_ChainLogger(FDT_LogMeta&& InMeta)
	: Meta(MoveTempIfPossible(InMeta))
{
}

UDT_ChainLogger::~UDT_ChainLogger()
{
	if (const auto Logger = UDT_Logger::Get())
	{
		Logger->WriteLine(MoveTempIfPossible(Meta), StringBuilder.ToString());
	}
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
	// TODO: Move to settings
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Error, true);
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Warning, true);
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Display, false);

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
}

UDT_Logger::~UDT_Logger()
{
}

void UDT_Logger::WriteLine(FDT_LogMeta&& Meta, FString&& Message)
{
	auto LogElement = FDT_LogElement();
	LogElement.Message = MoveTempIfPossible(Message);
	LogElement.File = MoveTempIfPossible(Meta.File);
	LogElement.Line = Meta.Line;
	LogElement.LogVerbosity = Meta.LogVerbosity;

	if (LogVerbosityWithStackTrace[LogElement.LogVerbosity])
	{
		LogElement.StackTrace = DT_GET_STACKTRACE();
	}

	if (Meta.ContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(Meta.ContextObject, EGetWorldErrorMode::ReturnNull);
		FString Prefix;
		if (World)
		{
			if (World->WorldType == EWorldType::PIE)
			{
				LogElement.NetMode = World->GetNetMode();
				if (LogElement.NetMode == NM_Client)
				{
					LogElement.NetId = GPlayInEditorID;
				}
			}
		}
	}

	auto* InsertedLogElement = &LoggerList.emplace_back(MoveTempIfPossible(LogElement));
	OnAddLogDelegate.Broadcast(InsertedLogElement);
}

void UDT_Logger::Breakpoint(FDT_LogMeta&& Meta)
{
	WriteLineFormat(MoveTempIfPossible(Meta), TEXT("BREAKPOINT"));
}

UDT_Logger::ConstIterator UDT_Logger::begin() const
{
	return std::rbegin(LoggerList);
}

UDT_Logger::ConstIterator UDT_Logger::end() const
{
	return std::rend(LoggerList);
}

UDT_ChainLogger UDT_Logger::CreateChainLogger(FDT_LogMeta&& Meta) const
{
	return UDT_ChainLogger(MoveTempIfPossible(Meta));
}

void UDT_Logger::ReloadLogFileFromSettingsClass()
{
#if 0
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
#endif
}

void UDT_Logger::UpdateLogVerbosityWithStackTrace(const ELogVerbosity::Type Verbosity, const bool bEnable)
{
	LogVerbosityWithStackTrace[Verbosity] = bEnable;
}

bool UDT_Logger::IsLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity)
{
	return LogVerbosityWithStackTrace[Verbosity];
}
