// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugTool/DT_LoggerStream.h"
#include "DebugTool/DT_LoggerSubsystem.h"

UDT_LoggerStream::UDT_LoggerStream()
{
	ensure(false);
}

UDT_LoggerStream::UDT_LoggerStream(UDT_LoggerSubsystem* InLoggerSubsystem, const FDT_LogMeta& InMeta)
	: LoggerSubsystem(InLoggerSubsystem)
	, Meta(&InMeta)
{}

UDT_LoggerStream::~UDT_LoggerStream()
{
	LoggerSubsystem->WriteLine(*Meta, StringBuilder.ToString());
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const char* Value)
{
	StringBuilder.Append(Value);
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const wchar_t* Value)
{
	StringBuilder.Append(Value);
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const int32 Value)
{
	StringBuilder.Append(FString::FromInt(Value));
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const float Value)
{
	StringBuilder.Append(FString::SanitizeFloat(Value));
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const bool Value)
{
	StringBuilder.Append(Value ? "True" : "False");
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const UObject* const Value)
{
	StringBuilder.Append(GetData(Value->GetName()));
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const FString& Value)
{
	StringBuilder.Append(Value);
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const FName& Value)
{
	StringBuilder.Append(Value.ToString());
	return *this;
}

UDT_LoggerStream& UDT_LoggerStream::operator<<(const FText& Value)
{
	StringBuilder.Append(Value.ToString());
	return *this;
}
