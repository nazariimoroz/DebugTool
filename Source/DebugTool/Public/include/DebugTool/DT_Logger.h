// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DT_LoggerSubsystem.h"
#include "DT_LoggerStream.h"


#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define DT_LOG_DEBUG_INFO __FILE__ "(" TO_STR_COV(__LINE__) "): "

#define DT_INIT_LOG_META(InLogVerbosity) FDT_LogMeta(__FILE__, __LINE__, InLogVerbosity)

#define DT_LOGSTR_OR_NOTHING1(...) __VA_OPT__(DT_LOGSTR_OR_NOTHING2(__VA_ARGS__))
#define DT_LOGSTR_OR_NOTHING2(Format, ...) TEXT(Format) __VA_OPT__(, __VA_ARGS__)

#define DT_DISPLAY(...) \
		([]() { \
			using TToRet = decltype(std::declval<UDT_LoggerSubsystem>().WriteLine(DT_INIT_LOG_META(ELogVerbosity::Display) __VA_OPT__(, DT_LOGSTR_OR_NOTHING1(__VA_ARGS__)))); \
			static constexpr auto LogMeta = DT_INIT_LOG_META(ELogVerbosity::Display); \
			if (const auto Logger = UDT_LoggerSubsystem::Get()) \
			{ \
				return Logger->WriteLine(LogMeta __VA_OPT__(, DT_LOGSTR_OR_NOTHING1(__VA_ARGS__))); \
			} \
			return TToRet{}; \
		})()

# if 0
#define DT_WARNING(...)																			\
		if (const auto Logger = UDT_LoggerSubsystem::Get())										\
			Logger->WriteLine(DT_INIT_LOG_META(ELogVerbosity::Warning)							\
                              __VA_OPT__(, DT_LOGSTR_OR_NOTHING1(__VA_ARGS__)))					\

#define DT_ERROR(...)																			\
		if (const auto Logger = UDT_LoggerSubsystem::Get())										\
			Logger->WriteLine(DT_INIT_LOG_META(ELogVerbosity::Error)							\
                              __VA_OPT__(, DT_LOGSTR_OR_NOTHING1(__VA_ARGS__)))					\

#define DT_BREAKPOINT()																			\
		if (const auto Logger = UDT_LoggerSubsystem::Get())										\
			Logger->WriteLine(DT_INIT_LOG_META(ELogVerbosity::Error), TEXT("BREAKPOINT"))					\


#define DT_RETURN_A_IF(EXPRESSION, TO_RET)		\
	do                                    		\
	{                                     		\
		if (EXPRESSION)	                		\
		{                                 		\
			DT_ERROR("{0}", TEXT(#EXPRESSION)); \
			return TO_RET;                		\
		}                                 		\
	}                                     		\
	while (false)

#define DT_RETURN_IF(EXPRESSION) DT_RETURN_A_IF(EXPRESSION, )

#define DT_CORETURN_A_IF(EXPRESSION, TO_RET)	\
	do                                    		\
	{                                     		\
		if (EXPRESSION)	                		\
		{                                 		\
			DT_ERROR("{0}", #EXPRESSION); 		\
			co_return TO_RET;              		\
		}                                 		\
	}                                     		\
	while (false)

#define DT_CORETURN_IF(EXPRESSION) DT_CORETURN_A_IF(EXPRESSION, )

#endif
