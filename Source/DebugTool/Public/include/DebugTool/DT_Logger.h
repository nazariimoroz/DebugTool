// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

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

#define DT_RETURN_A(Expression, TO_RET) do {        \
    if(!(Expression))                               \
    {                                               \
        DT_ERROR("{}", #Expression);                \
        return TO_RET;                              \
    }                                               \
    } while(false)

#define DT_RETURN(Expression) DT_RETURN_A(Expression, )

class FDebugToolModule;

struct FDT_LogElement
{
    FText LogText;
    FLinearColor LogVerbosityColor;

    void SetLogVerbosity(const ELogVerbosity::Type InLogVerbosity)
    {
        LogVerbosity = InLogVerbosity;
        switch (LogVerbosity)
        {
            case ELogVerbosity::Display:
                LogVerbosityColor = FLinearColor::White;
            break;
            case ELogVerbosity::Error:
                LogVerbosityColor = FLinearColor::Red;
            break;
            default:
                LogVerbosityColor = FLinearColor::Transparent;
            break;
        }
    }

    ELogVerbosity::Type GetLogVerbosity() const { return LogVerbosity; }

private:
    ELogVerbosity::Type LogVerbosity = ELogVerbosity::Display;
};

class DEBUGTOOL_API UDT_Logger
{
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

    TArray<FDT_LogElement> GetLastLogsInGame(int32 Count = -1) const;

public:
#pragma region Delegates
    bool bUseDelegates = false;

    TFunction<void(FDT_LogElement)> OnAddLogDelegate;
    TFunction<void(FDT_LogElement)> OnAddLogInGameDelegate;
#pragma endregion

protected:
    bool InGame() const { return WorldCount != 0; }

protected:
    int WorldCount = 0;
    bool bInited = false;

    TDoubleLinkedList<FDT_LogElement> LoggerListInGame;
    TDoubleLinkedList<FDT_LogElement> LoggerList;
};

