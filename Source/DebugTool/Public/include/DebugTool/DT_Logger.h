// FireProject by likapu company

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Kismet/KismetStringLibrary.h"
#include "Logging/StructuredLog.h"

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)


#define DT_DISPLAY_NO_LOGGER(Format, ...) do {       \
    UE_LOGFMT(LogTemp, Display, Format, __VA_ARGS__);\
    } while(false)

#define DT_ERROR_NO_LOGGER(Format, ...) do {       \
    UE_LOGFMT(LogTemp, Error, Format, __VA_ARGS__);\
    } while(false)

#define DT_BREAKPOINT_NO_LOGGER() do {                                                        \
    UE_LOGFMT(LogTemp, Error, "{0}: BREAKPOINT", TEXT(__FILE__ "(" TO_STR_COV(__LINE__) ")"));\
    } while(false)

#define DT_RETURN_A_NO_LOGGER(Expression, TO_RET) do {          \
    if(!(Expression))                                           \
    {                                                           \
        DT_ERROR_NO_LOGGER("{}", #Expression);                  \
        return TO_RET;                                          \
    }                                                           \
    } while(false)

#define DT_RETURN_NO_LOGGER(Expression) DT_RETURN_A(Expression, )


#define DT_DISPLAY(Format, ...) do {                                                                                                        \
    UE_LOGFMT(LogTemp, Display, Format, __VA_ARGS__);                                                                                       \
    if(const auto Logger = UDT_Logger::Get()) Logger->Display(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format), __VA_ARGS__);  \
    } while(false)

#define DT_ERROR(Format, ...) do {                                                                                                              \
    UE_LOGFMT(LogTemp, Error, Format, __VA_ARGS__);                                                                                             \
    if(const auto Logger = UDT_Logger::Get()) Logger->Error(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__, TEXT(Format), __VA_ARGS__);        \
    } while(false)

#define DT_BREAKPOINT() do {                                                                                                    \
    UE_LOGFMT(LogTemp, Error, "{0}: BREAKPOINT", TEXT(__FILE__ "(" TO_STR_COV(__LINE__) ")"));                                  \
    if(const auto Logger = UDT_Logger::Get()) Logger->Breakpoint(DT_GET_CATEGORY_BY_FILENAME(__FILE__), __LINE__);              \
    } while(false)

#define DT_RETURN_A(Expression, TO_RET) do {        \
    if(!(Expression))                               \
    {                                               \
        DT_ERROR("{}", #Expression);                \
        return TO_RET;                              \
    }                                               \
    } while(false)

#define DT_RETURN(Expression) DT_RETURN_A(Expression, )

constexpr std::string DT_GET_CATEGORY_BY_FILENAME(std::string InFileName)
{
    std::size_t BeginPos = InFileName.find_last_of("\\/");
    if(BeginPos == std::string::npos)
        BeginPos = 0;
    else
        BeginPos += 1;

    std::size_t EndPos = InFileName.find_last_of('.');
    if(BeginPos == std::string::npos)
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

    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogDelegate, FDT_LogElement);
    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogInGameDelegate, FDT_LogElement);

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
    void WriteLine(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line, const FString& Str);

    template<class ...T>
    void WriteLineFormat(const ELogVerbosity::Type LogVerbosity, const std::string& Category, const uint64 Line, const FStringView& Format, T ...Args)
    {
        const auto Str = FString::Format(Format.GetData(), FStringFormatOrderedArguments{Args...});

        WriteLine(LogVerbosity, Category, Line, Str);
    }

    template<class ...T>
    void Display(const std::string& Category, const uint64 Line, const FStringView& Format, T ...Args)
    {
        WriteLineFormat(ELogVerbosity::Display, Category, Line, Format, Args...);
    }

    template<class ...T>
    void Error(const std::string& Category, const uint64 Line, const FStringView& Format, T ...Args)
    {
        WriteLineFormat(ELogVerbosity::Error, Category, Line, Format, Args...);
    }

    void Breakpoint(const std::string& Category, const uint64 Line);

    TArray<FDT_LogElement> GetLastLogsInGame(int32 Count = -1) const;

public:
#pragma region Delegates
    bool bUseDelegates = false;

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
};

