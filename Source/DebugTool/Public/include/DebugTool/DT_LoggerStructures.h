// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <set>
#include <string>
#include <unordered_set>

#include "CoreMinimal.h"

/** Will store in std::list in Logger */
struct FDT_LogElement
{
	FString Message;
	std::string_view File;
	uint64 Line;
	ELogVerbosity::Type LogVerbosity = ELogVerbosity::Display;
	std::optional<FString> StackTrace;

	FString Tag = "#None";
};

/** Pass as parameter to most Log functions */
struct FDT_LogMeta
{
	FDT_LogMeta() = default;
	constexpr FDT_LogMeta(const std::string_view& InFile, uint64 InLine, ELogVerbosity::Type InLogVerbosity)
		: Category(ParseFilenameToCategory(InFile))
		, Line(InLine)
		, LogVerbosity(InLogVerbosity)
	{}

	std::string Category;
	uint64 Line;
	ELogVerbosity::Type LogVerbosity;

protected:
	constexpr std::string ParseFilenameToCategory(const std::string_view& InFileName)
	{
		std::optional<int32> BeginPos;
		std::optional<int32> EndPos;
		for (int i = InFileName.size() - 1; i >= 0; --i)
		{
			char Char = InFileName[i];
			if (!BeginPos && (Char == '\\' || Char == '/'))
			{
				BeginPos = i + 1;
			}
			if (!EndPos && Char == '.')
			{
				EndPos = i;
			}
		}
		if (!BeginPos)
			BeginPos = 0;
		if (!EndPos)
			return "UNKNOWN";

		const auto BV = BeginPos.value();
		const auto EV = EndPos.value();

		return std::string(InFileName.substr(BV, EV - BV));
	}
};
