// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugTool/DT_LoggerArray.h"

FDT_LoggerArray::FDT_LoggerArray()
	: CircularBuffer(100)
{}

FDT_LogElement& FDT_LoggerArray::Push(FDT_LogElement LogElement)
{
	CurrentIndex = CircularBuffer.GetNextIndex(CurrentIndex);
	return (CircularBuffer[CurrentIndex] = LogElement);
}

FDT_LoggerArray::FDT_LoggerArrayIterator FDT_LoggerArray::begin() const
{
	return FDT_LoggerArrayIterator(*this, CurrentIndex, false);
}

FDT_LoggerArray::FDT_LoggerArrayIterator FDT_LoggerArray::end() const
{
	return FDT_LoggerArrayIterator(*this, CurrentIndex, true);
}
