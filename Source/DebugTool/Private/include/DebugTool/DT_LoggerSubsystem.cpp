// FireProject by likapu company

#include "include/DebugTool/DT_LoggerSubsystem.h"

#include "DebugTool/DT_LoggerStream.h"

DEFINE_LOG_CATEGORY(LogDebugTool)

UDT_LoggerSubsystem::UDT_LoggerSubsystem()
{
	// TODO: Move to settings
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Error, true);
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Warning, true);
	LogVerbosityWithStackTrace.Add(ELogVerbosity::Display, false);
}

void UDT_LoggerSubsystem::WriteLine(const FDT_LogMeta& Meta, const FString& Message)
{
	auto LogElement = FDT_LogElement();
	LogElement.File = Meta.Category;
	LogElement.Line = Meta.Line;
	LogElement.LogVerbosity = Meta.LogVerbosity;
	LogElement.Message = FString::Printf(TEXT("%hs(%llu): %s"), LogElement.File.data(), LogElement.Line, *Message);

	if (LogVerbosityWithStackTrace[LogElement.LogVerbosity])
	{
		LogElement.StackTrace = DumpStacktrace();
	}

	if (!LogElement.Message.IsEmpty())
	{
		if (LogElement.Message[0] == '#')
		{
			int32 Index = 0;
			if (LogElement.Message.FindChar(' ', Index))
			{
				LogElement.Tag = LogElement.Message.Left(Index);
				LogElement.Message = LogElement.Message.Mid(Index + 1);
			}
		}
	}

	/** Base log */
	UE::Logging::Private::FStaticBasicLogDynamicData LOG_Dynamic;
	UE::Logging::Private::FStaticBasicLogRecord LOG_Static(
			*LogElement.Message,
			LogElement.File.data(),
			LogElement.Line,
			LogElement.LogVerbosity,
			LOG_Dynamic);
	UE::Logging::Private::BasicLog(LogDebugTool, &LOG_Static);

	/** Other log */
	const auto& InsertedLogElement = LoggerArray.Push(MoveTemp(LogElement));
	OnAddLogDelegate.Broadcast(&InsertedLogElement);
}

UDT_LoggerStream UDT_LoggerSubsystem::WriteLine(const FDT_LogMeta& Meta)
{
	return UDT_LoggerStream(this, Meta);
}

void UDT_LoggerSubsystem::UpdateLogVerbosityWithStackTrace(const ELogVerbosity::Type Verbosity, const bool bEnable)
{
	LogVerbosityWithStackTrace[Verbosity] = bEnable;
}

bool UDT_LoggerSubsystem::IsLogVerbosityWithStackTrace(ELogVerbosity::Type Verbosity)
{
	return LogVerbosityWithStackTrace[Verbosity];
}

FString UDT_LoggerSubsystem::DumpStacktrace() const
{
	const SIZE_T StackTraceSize = 65535;
	ANSICHAR* StackTrace = (ANSICHAR*)FMemory::SystemMalloc(StackTraceSize);

	StackTrace[0] = 0;
	const int32 NumStackFramesToIgnore = 2;
	FPlatformStackWalk::StackWalkAndDumpEx(StackTrace, StackTraceSize, NumStackFramesToIgnore,
		FGenericPlatformStackWalk::EStackWalkFlags::FlagsUsedWhenHandlingEnsure);

	FString ToRet = StackTrace;

	FMemory::SystemFree(StackTrace);

	return ToRet;
}
