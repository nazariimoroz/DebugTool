// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DT_LoggerStructures.h"

class UDT_LoggerSubsystem;

template <class T>
constexpr auto FTArrayCondition =
	std::is_arithmetic_v<T> || std::is_base_of_v<UObject, std::remove_pointer_t<T>>
	|| std::is_same_v<std::remove_reference_t<T>, FString> || std::is_same_v<std::remove_reference_t<T>, FName>
	|| std::is_same_v<std::remove_reference_t<T>, FText>;

class DEBUGTOOL_API UDT_LoggerStream final
{
public:
	UDT_LoggerStream();
	UDT_LoggerStream(UDT_LoggerSubsystem* InLoggerSubsystem, const FDT_LogMeta& InMeta);
	~UDT_LoggerStream();

	UDT_LoggerStream& operator<<(const char* Value);
	UDT_LoggerStream& operator<<(const wchar_t* Value);
	UDT_LoggerStream& operator<<(int32 Value);
	UDT_LoggerStream& operator<<(float Value);
	UDT_LoggerStream& operator<<(bool Value);

	UDT_LoggerStream& operator<<(const UObject* const Value);
	UDT_LoggerStream& operator<<(const FString& Value);
	UDT_LoggerStream& operator<<(const FName& Value);
	UDT_LoggerStream& operator<<(const FText& Value);

	template <class T, std::enable_if_t<FTArrayCondition<T>, bool>  = true>
	UDT_LoggerStream& operator<<(const TArray<T>& Array)
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
	UDT_LoggerSubsystem* LoggerSubsystem;
	const FDT_LogMeta* Meta;

	FStringBuilderBase StringBuilder;
};

