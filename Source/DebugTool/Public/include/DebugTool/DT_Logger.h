// FireProject by likapu company

#pragma once

#include <fstream>
#include <string>

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogDebugTool, All, All)

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#pragma region NoLoggerMacros
#define DT_DISPLAY_NO_LOGGER(Format, ...) do {       \
    UE_LOGFMT(LogDebugTool, Display, Format __VA_OPT__(,) __VA_ARGS__);\
    } while(false)

#define DT_ERROR_NO_LOGGER(Format, ...) do {       \
    UE_LOGFMT(LogDebugTool, Error, Format __VA_OPT__(,) __VA_ARGS__);\
    } while(false)

#define DT_BREAKPOINT_NO_LOGGER() do {                                                        \
    UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(__FILE__ "(" TO_STR_COV(__LINE__) ")"));\
    } while(false)

#define DT_RETURN_A_NO_LOGGER(Expression, TO_RET) do {          \
    if(!(Expression))                                           \
    {                                                           \
        DT_ERROR_NO_LOGGER("{0}", #Expression);                 \
        return TO_RET;                                          \
    }                                                           \
    } while(false)

#define DT_RETURN_NO_LOGGER(Expression) DT_RETURN_A_NO_LOGGER(Expression, )
#pragma endregion NoLoggerMacros

#pragma region BaseMacros
#define DT_DISPLAY(Format, ...) do {                                                                                                        \
    UE_LOGFMT(LogDebugTool, Display, Format __VA_OPT__(,) __VA_ARGS__);                                                                                       \
    if(const auto Logger = UDT_Logger::Get()) Logger->Display(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format) __VA_OPT__(,) __VA_ARGS__);  \
    } while(false)

#define DT_ERROR(Format, ...) do {                                                                                                              \
    UE_LOGFMT(LogDebugTool, Error, Format __VA_OPT__(,) __VA_ARGS__);                                                                                             \
    if(const auto Logger = UDT_Logger::Get()) Logger->Error(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format) __VA_OPT__(,) __VA_ARGS__);        \
    } while(false)

// DEPRECATED
#define DT_DISPLAY_A(Message) do {                                                                                                        \
    UE_LOGFMT(LogDebugTool, Display, "{0}", Message);                                                                                       \
    if(const auto Logger = UDT_Logger::Get()) Logger->Display(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT("{0}"), Message);  \
    } while(false)

// DEPRECATED
#define DT_ERROR_A(Message) do {                                                                                                              \
    UE_LOGFMT(LogDebugTool, Error, "{0}", Message);                                                                                             \
    if(const auto Logger = UDT_Logger::Get()) Logger->Error(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT("{0}"), Message);        \
    } while(false)

#define DT_BREAKPOINT() do {                                                                                                    \
    UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(__FILE__ "(" TO_STR_COV(__LINE__) ")"));                                  \
    if(const auto Logger = UDT_Logger::Get()) Logger->Breakpoint(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__);              \
    } while(false)
#pragma endregion BaseMacros

#pragma region ChainedMacros
#define DT_CHAINED_DISPLAY() \
    if(const auto Logger = UDT_Logger::Get()) Logger->CreateChainLogger(ELogVerbosity::Display ,DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__)

#define DT_CHAINED_ERROR() \
    if(const auto Logger = UDT_Logger::Get()) Logger->CreateChainLogger(ELogVerbosity::Error ,DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__)
#pragma endregion ChainedMacros

#pragma region ReturnMacros
#define DT_RETURN_A(Expression, TO_RET) do {        \
    if(!(Expression))                               \
    {                                               \
        DT_ERROR("{}", #Expression);                \
        return TO_RET;                              \
    }                                               \
    } while(false)

#define DT_RETURN(Expression) DT_RETURN_A(Expression, )
#pragma endregion ReturnMacros

constexpr std::string DT_GET_CATEGORY_BY_FILENAME(std::string InFileName)
{
    std::size_t BeginPos = InFileName.find_last_of("\\/");
    if (BeginPos == std::string::npos)
        BeginPos = 0;
    else
        BeginPos += 1;

    std::size_t EndPos = InFileName.find_last_of('.');
    if (BeginPos == std::string::npos)
    {
        return "UNKNOWN";
    }

    return InFileName.substr(BeginPos, EndPos - BeginPos);
}

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
            case ELogVerbosity::Display: LogVerbosityColor = FLinearColor::White;
                break;
            case ELogVerbosity::Error: LogVerbosityColor = FLinearColor::Red;
                break;
            default: LogVerbosityColor = FLinearColor::Transparent;
                break;
        }
    }

    ELogVerbosity::Type GetLogVerbosity() const { return LogVerbosity; }

private:
    ELogVerbosity::Type LogVerbosity = ELogVerbosity::Display;
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
    UDT_ChainLogger(const ELogVerbosity::Type InLogVerbosity, const std::string& InCategory, const uint64 InLine);
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
    std::string Category;
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
    void WriteLine(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line, const FString& Str);

    template <class... T>
    void WriteLineFormat(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line, const FStringView& Format,
        T... Args)
    {
        const auto Str = FString::Format(Format.GetData(), FStringFormatOrderedArguments{Args...});

        WriteLine(LogVerbosity, Category, Line, Str);
    }

    template <class... T>
    void Display(const std::string& Category, const uint64 Line, const FStringView& Format, T... Args)
    {
        WriteLineFormat(ELogVerbosity::Display, Category, Line, Format, Args...);
    }

    template <class... T>
    void Error(const std::string& Category, const uint64 Line, const FStringView& Format, T... Args)
    {
        WriteLineFormat(ELogVerbosity::Error, Category, Line, Format, Args...);
    }

    void Breakpoint(const std::string& Category, const uint64 Line);

    TArray<FDT_LogElement> GetLastLogsInGame(int32 Count = -1) const;

    UDT_ChainLogger CreateChainLogger(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line) const;

    void ReloadLogFileFromSettingsClass();

public:
    bool bUseDelegates = false;
    bool bUseLoggerFile = false;

#pragma region Delegates
    FDT_OnAddLogDelegate OnAddLogDelegate;
    FDT_OnAddLogInGameDelegate OnAddLogInGameDelegate;
#pragma endregion

protected:
    bool InGame() const { return WorldCount != 0; }

protected:
    int WorldCount = 0;
    bool bInited = false;

    TDoubleLinkedList<FDT_LogElement> LoggerListInGame;
    TDoubleLinkedList<FDT_LogElement> LoggerList;

    std::wofstream LoggerFile;
};
