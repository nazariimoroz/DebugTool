// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "UObject/Object.h"
#include "functional"
#include "DT_Logger.generated.h"

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define DT_DISPLAY(Format, ...) do {                                                                            \
    UE_LOGFMT(LogTemp, Display, Format, __VA_ARGS__);                                                           \
    if(const auto Logger = UDT_Logger::Get()) Logger->Display(__FILE__, __LINE__, TEXT(Format), __VA_ARGS__);   \
    } while(false)

#define DT_ERROR(Format, ...) do {                                                                              \
    UE_LOGFMT(LogTemp, Error, Format, __VA_ARGS__);                                                             \
    if(const auto Logger = UDT_Logger::Get()) Logger->Error(__FILE__, __LINE__, TEXT(Format), __VA_ARGS__);     \
    } while(false)

#define DT_BREAKPOINT() do {                                                                    \
    UE_LOGFMT(LogTemp, Error, "{0}: BREAKPOINT", TEXT(__FILE__ "(" TO_STR_COV(__LINE__) ")"));  \
    if(const auto Logger = UDT_Logger::Get()) Logger->Breakpoint(__FILE__, __LINE__);           \
    } while(false)


class FDebugToolModule;

UCLASS()
class DEBUGTOOL_API UDT_Logger : public UObject
{
    GENERATED_BODY()

    friend FDebugToolModule;

#pragma region Singleton
protected:
    UDT_Logger();

public:
    static UDT_Logger* Get()
    {
        if(!Singleton)
            UE_LOG(LogTemp, Error, TEXT("UDT_Logger is not inited"));
        return Singleton;
    }

protected:
    static UDT_Logger* Singleton;
#pragma endregion

public:
    void WriteLine(const ELogVerbosity::Type LogVerbosity, const FString& FilePath, const uint64 Line, const FString& Str);

    template<class ...T>
    void WriteLineFormat(const ELogVerbosity::Type LogVerbosity, const FString& FilePath, const uint64 Line, const FStringView& Format, T ...Args)
    {
        const auto Str = FString::Format(Format.GetData(), FStringFormatOrderedArguments{Args...});

        WriteLine(LogVerbosity, FilePath, Line, Str);
    }

    template<class ...T>
    void Display(const FString& FilePath, const uint64 Line, const FStringView& Format, T ...Args)
    {
        WriteLineFormat(ELogVerbosity::Display, FilePath, Line, Format, Args...);
    }

    template<class ...T>
    void Error(const FString& FilePath, const uint64 Line, const FStringView& Format, T ...Args)
    {
        WriteLineFormat(ELogVerbosity::Error, FilePath, Line, Format, Args...);
    }

    void Breakpoint(const FString& FilePath, const uint64 Line);

public:
#pragma region Delegates
    std::function<void(ELogVerbosity::Type /*LogVerbosity*/, FString /*Message*/)> OnAddLogDelegate;
#pragma endregion

};

