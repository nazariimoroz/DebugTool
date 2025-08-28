// FireProject by likapu company

#pragma once

#include <fstream>
#include <string>

#include "CoreMinimal.h"
#include "DT_LoggerArray.h"
#include "DT_LoggerStructures.h"
#include "Logging/StructuredLog.h"
#include "format"
#include "DT_LoggerSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDebugTool, All, All)

class UDT_LoggerStream;

UCLASS()
class DEBUGTOOL_API UDT_LoggerSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogDelegate, const FDT_LogElement* /*Log Element*/);

public:
    FDT_OnAddLogDelegate OnAddLogDelegate;

protected:
    UDT_LoggerSubsystem();

public:
    static UDT_LoggerSubsystem* Get()
    {
    	return GEngine->GetEngineSubsystem<UDT_LoggerSubsystem>();
    }

public:
    void WriteLine(const FDT_LogMeta& Meta, const FString& Message);

    template <class... T>
    void WriteLine(const FDT_LogMeta& Meta, const std::wstring_view& Format, T&&... Args)
    {
        const std::wstring TempMessage = std::vformat(Format,
                                                      std::make_wformat_args(std::forward<T>(Args)...));
        auto Message = FString(TempMessage.c_str());

        WriteLine(Meta, Message);
    }

    UDT_LoggerStream WriteLine(const FDT_LogMeta& Meta);

    const FDT_LoggerArray& GetLoggerArray() const { return LoggerArray; };

    void UpdateLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity, bool bEnable);
    bool IsLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity);

protected:
    FString DumpStacktrace() const;

protected:
	FDT_LoggerArray LoggerArray;
    TMap<ELogVerbosity::Type, bool> LogVerbosityWithStackTrace;
};