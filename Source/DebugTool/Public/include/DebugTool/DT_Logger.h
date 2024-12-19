// FireProject by likapu company

#pragma once

#include <fstream>
#include <string>

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogDebugTool, All, All)

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define DT_LOG_DEBUG_INFO __FILE__ "(" TO_STR_COV(__LINE__) "): "

#pragma region NoLoggerMacros
#define DT_DISPLAY_NO_LOGGER(Format, ...) do {                                                  \
    UE_LOGFMT(LogDebugTool, Display, DT_LOG_DEBUG_INFO Format __VA_OPT__(,) __VA_ARGS__);       \
    } while(false)

#define DT_ERROR_NO_LOGGER(Format, ...) do {                                                    \
    UE_LOGFMT(LogDebugTool, Error, DT_LOG_DEBUG_INFO Format __VA_OPT__(,) __VA_ARGS__);         \
    } while(false)

#define DT_BREAKPOINT_NO_LOGGER() do {                                                          \
    UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(DT_LOG_DEBUG_INFO));                 \
    } while(false)

#define DT_RETURN_A_NO_LOGGER(Expression, TO_RET) do {                                          \
    if(!(Expression))                                                                           \
    {                                                                                           \
        DT_ERROR_NO_LOGGER("{0}", #Expression);                                                 \
        return TO_RET;                                                                          \
    }                                                                                           \
    } while(false)

#define DT_RETURN_NO_LOGGER(Expression) DT_RETURN_A_NO_LOGGER(Expression, )
#pragma endregion NoLoggerMacros

#pragma region BaseMacros
#define DT_DISPLAY(Format, ...) do {                                                                                                                        \
    UE_LOGFMT(LogDebugTool, Display, DT_LOG_DEBUG_INFO Format __VA_OPT__(,) __VA_ARGS__);                                                                   \
    if(const auto Logger = UDT_Logger::Get()) Logger->Display(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format) __VA_OPT__(,) __VA_ARGS__);     \
    } while(false)

#define DT_WARNING(Format, ...) do {                                                                                                                        \
    UE_LOGFMT(LogDebugTool, Warning, DT_LOG_DEBUG_INFO Format __VA_OPT__(,) __VA_ARGS__);                                                                   \
    if(const auto Logger = UDT_Logger::Get()) Logger->Warning(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format) __VA_OPT__(,) __VA_ARGS__);     \
    } while(false)

#define DT_ERROR(Format, ...) do {                                                                                                                          \
    UE_LOGFMT(LogDebugTool, Error, DT_LOG_DEBUG_INFO Format __VA_OPT__(,) __VA_ARGS__);                                                                     \
    if(const auto Logger = UDT_Logger::Get()) Logger->Error(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format) __VA_OPT__(,) __VA_ARGS__);       \
    } while(false)

#define DT_BREAKPOINT() do {                                                                                                                                \
    UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(DT_LOG_DEBUG_INFO));                                                                             \
    if(const auto Logger = UDT_Logger::Get()) Logger->Breakpoint(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__);                                          \
    } while(false)
#pragma endregion BaseMacros

#pragma region ChainedMacros
#define DT_CHAINED_DISPLAY() \
    if(const auto Logger = UDT_Logger::Get()) Logger->CreateChainLogger(ELogVerbosity::Display ,DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__)

#define DT_CHAINED_WARNING() \
    if(const auto Logger = UDT_Logger::Get()) Logger->CreateChainLogger(ELogVerbosity::Warning ,DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__)

#define DT_CHAINED_ERROR() \
    if(const auto Logger = UDT_Logger::Get()) Logger->CreateChainLogger(ELogVerbosity::Error ,DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__)
#pragma endregion ChainedMacros

#pragma region ReturnMacros
#define DT_RETURN_A(Expression, TO_RET) do {        \
    if(!(Expression))                               \
    {                                               \
        DT_ERROR("{0}", #Expression);               \
        return TO_RET;                              \
    }                                               \
    } while(false)

#define DT_RETURN(Expression) DT_RETURN_A(Expression, )
#pragma endregion ReturnMacros

FORCEINLINE FString DT_GET_CATEGORY_BY_FILENAME(const TStringView<char>& InFileName)
{
    TOptional<int32> BeginPos;
    TOptional<int32> EndPos;
    for (int i = InFileName.Len() - 1; i >= 0; --i)
    {
        TCHAR Char = InFileName[i];
        if (!BeginPos && (Char == TEXT('\\') || Char == TEXT('/')))
        {
            BeginPos = i + 1;
        }
        if (!EndPos && Char == TEXT('.'))
        {
            EndPos = i;
        }
    }
    if (!BeginPos) BeginPos = 0;
    if (!EndPos) return "UNKNOWN";

    const auto BV = BeginPos.GetValue();
    const auto EV = EndPos.GetValue();

    return FString{ InFileName.Mid(BV, EV - BV) };
}

FORCEINLINE FString DT_GET_STACKTRACE()
{
    const SIZE_T StackTraceSize = 65535;
    ANSICHAR* StackTrace = (ANSICHAR*)FMemory::SystemMalloc(StackTraceSize);

    StackTrace[0] = 0;
    const int32 NumStackFramesToIgnore = 2;
    FPlatformStackWalk::StackWalkAndDumpEx(StackTrace, StackTraceSize, NumStackFramesToIgnore, FGenericPlatformStackWalk::EStackWalkFlags::FlagsUsedWhenHandlingEnsure);

    FString ToRet = StackTrace;

    FMemory::SystemFree(StackTrace);

    return ToRet;
}

class FDebugToolModule;

struct FDT_LogElement
{
    FString LogText;
    FString Category;
    uint64 Line;
    ELogVerbosity::Type LogVerbosity = ELogVerbosity::Display;
    TOptional<FString> StackTrace;

    mutable FString FullText;

    FString GetFullText() const
    {
        if(!FullText.IsEmpty()) return FullText;

        FullText = FString::Printf(TEXT("%s(%llu): %s"), *Category, Line, *LogText);
        if (StackTrace)
        {
            FullText += TEXT("\n\n");
            FullText += *StackTrace;
        }

        return FullText;
    }
};

template <class T>
constexpr auto FTArrayCondition =
    std::is_arithmetic_v<T> ||
    std::is_base_of_v<UObject, std::remove_pointer_t<T>> ||
    std::is_same_v<std::remove_reference_t<T>, FString> ||
    std::is_same_v<std::remove_reference_t<T>, FName> ||
    std::is_same_v<std::remove_reference_t<T>, FText>;

class DEBUGTOOL_API UDT_ChainLogger final
{
public:
    UDT_ChainLogger(const ELogVerbosity::Type InLogVerbosity, FString&& InCategory, const uint64 InLine);
    ~UDT_ChainLogger();

    UDT_ChainLogger& operator<<(const char* Value);
    UDT_ChainLogger& operator<<(const wchar_t* Value);
    UDT_ChainLogger& operator<<(int32 Value);
    UDT_ChainLogger& operator<<(float Value);
    UDT_ChainLogger& operator<<(bool Value);

    UDT_ChainLogger& operator<<(const UObject* const Value);
    UDT_ChainLogger& operator<<(const FString& Value);
    UDT_ChainLogger& operator<<(const FName& Value);
    UDT_ChainLogger& operator<<(const FText& Value);

    template <class T, std::enable_if_t<FTArrayCondition<T>, bool>  = true>
    UDT_ChainLogger& operator<<(const TArray<T>& Array)
    {
        StringBuilder.Append("[ ");

        int Index = 0;
        for (const auto& Value : Array)
        {
            operator<<(Value);

            if (Array.Num() - 1 != Index)
                StringBuilder.Append(", ");

            Index += 1;
        }
        StringBuilder.Append(" ]");
        return *this;
    }

private:
    ELogVerbosity::Type LogVerbosity;
    FString Category;
    uint64 Line;

    FStringBuilderBase StringBuilder;
};

class DEBUGTOOL_API UDT_Logger
{
    friend FDebugToolModule;

    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogDelegate, FDT_LogElement);
    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogInGameDelegate, FDT_LogElement);

#pragma region Singleton

protected:
    UDT_Logger();
    ~UDT_Logger();

public:
    static UDT_Logger* Get()
    {
        if (!Singleton)
            UE_LOG(LogTemp, Error, TEXT("UDT_Logger is not inited"));
        return Singleton;
    }

protected:
    static UDT_Logger* Singleton;
#pragma endregion

public:
    void WriteLine(const ELogVerbosity::Type LogVerbosity,
        FString&& Category,
        const uint64 Line,
        FString&& Str);

    template <class... T>
    void WriteLineFormat(const ELogVerbosity::Type LogVerbosity,
        FString&& Category,
        const uint64 Line,
        const FStringView& Format,
        T... Args)
    {
        auto Str = FString::Format(Format.GetData(), FStringFormatOrderedArguments{Args...});

        WriteLine(LogVerbosity, MoveTempIfPossible(Category), Line, MoveTempIfPossible(Str));
    }

    template <class... T>
    void Display(FString&& Category, const uint64 Line, const FStringView& Format, T... Args)
    {
        WriteLineFormat(ELogVerbosity::Display, MoveTempIfPossible(Category), Line, Format, Args...);
    }

    template <class... T>
    void Warning(FString&& Category, const uint64 Line, const FStringView& Format, T... Args)
    {
        WriteLineFormat(ELogVerbosity::Warning, MoveTempIfPossible(Category), Line, Format, Args...);
    }

    template <class... T>
    void Error(FString&& Category, const uint64 Line, const FStringView& Format, T... Args)
    {
        WriteLineFormat(ELogVerbosity::Error, MoveTempIfPossible(Category), Line, Format, Args...);
    }

    void Breakpoint(FString&& Category, const uint64 Line);

    TArray<FDT_LogElement> GetLastLogs(int32 Count = -1) const;

    UDT_ChainLogger CreateChainLogger(const ELogVerbosity::Type LogVerbosity, FString&& Category, const uint64 Line) const;

    void ReloadLogFileFromSettingsClass();

public:
    FDT_OnAddLogDelegate OnAddLogDelegate;

public:
    bool bUseDelegates = false;
    bool bUseLoggerFile = false;

protected:
    bool bInited = false;

    TDoubleLinkedList<FDT_LogElement> LoggerList;

    std::wofstream LoggerFile;

};
