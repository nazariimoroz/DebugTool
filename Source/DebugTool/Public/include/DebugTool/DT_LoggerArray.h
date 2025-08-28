// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DT_LoggerStructures.h"

class DEBUGTOOL_API FDT_LoggerArray
{
	struct FDT_LoggerArrayIterator
	{
		const FDT_LoggerArray& Array;
		int32 StartIndex;
		int32 CurrentIndex;
		bool bLoopPassed = false;

		explicit FDT_LoggerArrayIterator(const FDT_LoggerArray& InArray, int32 InStartIndex, bool bInLoopPassed)
			: Array(InArray)
			, StartIndex(InStartIndex)
			, CurrentIndex(InStartIndex)
			, bLoopPassed(bInLoopPassed)
		{}

		const FDT_LogElement& operator*() const
		{
			return Array.CircularBuffer[CurrentIndex];
		}

		FDT_LoggerArrayIterator& operator++() {
			if (bLoopPassed) return *this;

			CurrentIndex = Array.CircularBuffer.GetNextIndex(CurrentIndex);
			if (CurrentIndex == StartIndex)
			{
				bLoopPassed = true;
			}
			return *this;
		}

		bool operator==(const FDT_LoggerArrayIterator& Other) const
		{
			return CurrentIndex == Other.CurrentIndex && bLoopPassed == Other.bLoopPassed;
		}
	};

public:
	FDT_LoggerArray();

	FDT_LogElement& Push(FDT_LogElement LogElement);

	FDT_LoggerArrayIterator begin() const;
	FDT_LoggerArrayIterator end() const;

private:
	TCircularBuffer<FDT_LogElement> CircularBuffer;
	int32 CurrentIndex = 0;

};
