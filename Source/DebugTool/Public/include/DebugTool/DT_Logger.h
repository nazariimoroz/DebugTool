// FireProject by likapu company

#pragma once

#include <fstream>
#include <string>

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"

#include <list>

#pragma region Utils
DEFINE_LOG_CATEGORY_STATIC(LogDebugTool, All, All)

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define DT_LOG_DEBUG_INFO __FILE__ "(" TO_STR_COV(__LINE__) "): "

#define DT_INIT_LOG_META(InLogVerbosity)                     \
	FDT_LogMeta                                              \
	{                                                        \
		.File = DT_FORMAT_FILENAME(__FILE__),                \
		.ContextObject = dynamic_cast<const UObject*>(this), \
		.Line = __LINE__,                                    \
		.LogVerbosity = InLogVerbosity                       \
	}

#define DT_INIT_LOG_META_NO_THIS(InLogVerbosity) \
	FDT_LogMeta                                  \
	{                                            \
		.File = DT_FORMAT_FILENAME(__FILE__),    \
		.ContextObject = nullptr,                \
		.Line = __LINE__,                        \
		.LogVerbosity = InLogVerbosity           \
	}

#pragma endregion Utils

#pragma region NoLoggerMacros
#define DT_DISPLAY_NO_LOGGER(Format, ...)                                                      \
	do                                                                                         \
	{                                                                                          \
		UE_LOGFMT(LogDebugTool, Display, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
	}                                                                                          \
	while (false)

#define DT_ERROR_NO_LOGGER(Format, ...)                                                      \
	do                                                                                       \
	{                                                                                        \
		UE_LOGFMT(LogDebugTool, Error, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
	}                                                                                        \
	while (false)

#define DT_BREAKPOINT_NO_LOGGER()                                                   \
	do                                                                              \
	{                                                                               \
		UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(DT_LOG_DEBUG_INFO)); \
	}                                                                               \
	while (false)

#define DT_RETURN_A_NO_LOGGER(Expression, TO_RET)   \
	do                                              \
	{                                               \
		if (!(Expression))                          \
		{                                           \
			DT_ERROR_NO_LOGGER("{0}", #Expression); \
			return TO_RET;                          \
		}                                           \
	}                                               \
	while (false)

#define DT_RETURN_NO_LOGGER(Expression) DT_RETURN_A_NO_LOGGER(Expression, )
#pragma endregion NoLoggerMacros

#pragma region BaseMacros
#define DT_DISPLAY(Format, ...)                                                                \
	do                                                                                         \
	{                                                                                          \
		UE_LOGFMT(LogDebugTool, Display, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
		if (const auto Logger = UDT_Logger::Get())                                             \
			Logger->WriteLineFormat(DT_INIT_LOG_META(ELogVerbosity::Display),                  \
				TEXT(Format) __VA_OPT__(, ) __VA_ARGS__);                                      \
	}                                                                                          \
	while (false)

#define DT_WARNING(Format, ...)                                                                \
	do                                                                                         \
	{                                                                                          \
		UE_LOGFMT(LogDebugTool, Warning, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
		if (const auto Logger = UDT_Logger::Get())                                             \
			Logger->WriteLineFormat(DT_INIT_LOG_META(ELogVerbosity::Warning),                  \
				TEXT(Format) __VA_OPT__(, ) __VA_ARGS__);                                      \
	}                                                                                          \
	while (false)

#define DT_ERROR(Format, ...)                                                                                         \
	do                                                                                                                \
	{                                                                                                                 \
		UE_LOGFMT(LogDebugTool, Error, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__);                          \
		if (const auto Logger = UDT_Logger::Get())                                                                    \
			Logger->WriteLineFormat(DT_INIT_LOG_META(ELogVerbosity::Error), TEXT(Format) __VA_OPT__(, ) __VA_ARGS__); \
	}                                                                                                                 \
	while (false)

#define DT_BREAKPOINT()                                                             \
	do                                                                              \
	{                                                                               \
		UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(DT_LOG_DEBUG_INFO)); \
		if (const auto Logger = UDT_Logger::Get())                                  \
			Logger->Breakpoint(DT_INIT_LOG_META(ELogVerbosity::Error));             \
	}                                                                               \
	while (false)

#define DT_DISPLAY_NO_CONTEXT(Format, ...)                                                     \
	do                                                                                         \
	{                                                                                          \
		UE_LOGFMT(LogDebugTool, Display, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
		if (const auto Logger = UDT_Logger::Get())                                             \
			Logger->WriteLineFormat(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Display),          \
				TEXT(Format) __VA_OPT__(, ) __VA_ARGS__);                                      \
	}                                                                                          \
	while (false)

#define DT_WARNING_NO_CONTEXT(Format, ...)                                                     \
	do                                                                                         \
	{                                                                                          \
		UE_LOGFMT(LogDebugTool, Warning, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
		if (const auto Logger = UDT_Logger::Get())                                             \
			Logger->WriteLineFormat(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Warning),          \
				TEXT(Format) __VA_OPT__(, ) __VA_ARGS__);                                      \
	}                                                                                          \
	while (false)

#define DT_ERROR_NO_CONTEXT(Format, ...)                                                     \
	do                                                                                       \
	{                                                                                        \
		UE_LOGFMT(LogDebugTool, Error, DT_LOG_DEBUG_INFO Format __VA_OPT__(, ) __VA_ARGS__); \
		if (const auto Logger = UDT_Logger::Get())                                           \
			Logger->WriteLineFormat(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Error),          \
				TEXT(Format) __VA_OPT__(, ) __VA_ARGS__);                                    \
	}                                                                                        \
	while (false)

#define DT_BREAKPOINT_NO_CONTEXT()                                                  \
	do                                                                              \
	{                                                                               \
		UE_LOGFMT(LogDebugTool, Error, "{0}: BREAKPOINT", TEXT(DT_LOG_DEBUG_INFO)); \
		if (const auto Logger = UDT_Logger::Get())                                  \
			Logger->Breakpoint(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Error));     \
	}                                                                               \
	while (false)

#pragma endregion BaseMacros

#pragma region ChainedMacros
#define DT_CHAINED_DISPLAY()                   \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META(ELogVerbosity::Display))

#define DT_CHAINED_WARNING()                   \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META(ELogVerbosity::Warning))

#define DT_CHAINED_ERROR()                     \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META(ELogVerbosity::Error))

#define DT_CHAINED_DISPLAY_NO_CONTEXT()        \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Display))

#define DT_CHAINED_WARNING_NO_CONTEXT()        \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Warning))

#define DT_CHAINED_ERROR_NO_CONTEXT()          \
	if (const auto Logger = UDT_Logger::Get()) \
	Logger->CreateChainLogger(DT_INIT_LOG_META_NO_THIS(ELogVerbosity::Error))

#pragma endregion ChainedMacros

#pragma region ReturnMacros
#define DT_RETURN_A(Expression, TO_RET)   \
	do                                    \
	{                                     \
		if (!(Expression))                \
		{                                 \
			DT_ERROR("{0}", #Expression); \
			return TO_RET;                \
		}                                 \
	}                                     \
	while (false)

#define DT_RETURN(Expression) DT_RETURN_A(Expression, )

#pragma endregion ReturnMacros

FORCEINLINE FString DT_FORMAT_FILENAME(const TStringView<char>& InFileName)
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
	if (!BeginPos)
		BeginPos = 0;
	if (!EndPos)
		return "UNKNOWN";

	const auto BV = BeginPos.GetValue();
	const auto EV = EndPos.GetValue();

	return FString{ InFileName.Mid(BV, EV - BV) };
}

FORCEINLINE FString DT_GET_STACKTRACE()
{
	const SIZE_T StackTraceSize = 65535;
	ANSICHAR*	 StackTrace = (ANSICHAR*)FMemory::SystemMalloc(StackTraceSize);

	StackTrace[0] = 0;
	const int32 NumStackFramesToIgnore = 2;
	FPlatformStackWalk::StackWalkAndDumpEx(StackTrace, StackTraceSize, NumStackFramesToIgnore,
		FGenericPlatformStackWalk::EStackWalkFlags::FlagsUsedWhenHandlingEnsure);

	FString ToRet = StackTrace;

	FMemory::SystemFree(StackTrace);

	return ToRet;
}

class FDebugToolModule;

/** Will store in std::list in Logger */
struct FDT_LogElement
{
	FString				Message;
	FString				File;
	uint64				Line;
	ELogVerbosity::Type LogVerbosity = ELogVerbosity::Display;
	TOptional<FString>	StackTrace;

	ENetMode NetMode = NM_MAX;
	/** Will be setted only if NetMode == Client */
	int32 NetId;

	FString Tag = "#None";
};

/** Pass as parameter to most Log functions */
struct FDT_LogMeta
{
	FString				File;
	const UObject*		ContextObject = nullptr;
	uint64				Line;
	ELogVerbosity::Type LogVerbosity;
};

template <class T>
constexpr auto FTArrayCondition =
	std::is_arithmetic_v<T> || std::is_base_of_v<UObject, std::remove_pointer_t<T>>
	|| std::is_same_v<std::remove_reference_t<T>, FString> || std::is_same_v<std::remove_reference_t<T>, FName>
	|| std::is_same_v<std::remove_reference_t<T>, FText>;

class DEBUGTOOL_API UDT_ChainLogger final
{
public:
	UDT_ChainLogger(FDT_LogMeta&& InMeta);
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

	template <class T, std::enable_if_t<FTArrayCondition<T>, bool> = true>
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
	FDT_LogMeta&& Meta;

	FStringBuilderBase StringBuilder;
};

class DEBUGTOOL_API UDT_Logger
{
	friend FDebugToolModule;

	DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnAddLogDelegate, const FDT_LogElement* /*Log Element*/);

public:
	FDT_OnAddLogDelegate OnAddLogDelegate;

public:
	using ConstIterator = std::list<FDT_LogElement>::const_iterator;

protected:
#pragma region Singleton
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
	void WriteLine(FDT_LogMeta&& Meta, FString&& Message);

	template <class... T>
	void WriteLineFormat(FDT_LogMeta&& Meta, const FStringView& Format, T... Args)
	{
		auto Message = FString::Format(Format.GetData(), FStringFormatOrderedArguments{ Args... });

		WriteLine(MoveTempIfPossible(Meta), MoveTempIfPossible(Message));
	}

	void Breakpoint(FDT_LogMeta&& Meta);

	ConstIterator begin() const;
	ConstIterator end() const;

	UDT_ChainLogger CreateChainLogger(FDT_LogMeta&& Meta) const;

	void ReloadLogFileFromSettingsClass();

	void UpdateLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity, bool bEnable);
	bool IsLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity);

protected:
	std::list<FDT_LogElement> LoggerList;

	TMap<ELogVerbosity::Type, bool> LogVerbosityWithStackTrace;
};
