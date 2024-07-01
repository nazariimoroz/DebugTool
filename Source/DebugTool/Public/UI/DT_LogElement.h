// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_LogElement.generated.h"

class UTextBlock;

UCLASS(Blueprintable, BlueprintType)
class UDT_LogElementInfo : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    FLinearColor GetLogTypeColor() const
    {
        switch (LogVerbosity)
        {
            case ELogVerbosity::Display:
                return FLinearColor::White;
            case ELogVerbosity::Error:
                return FLinearColor::Red;
            default:
                return FLinearColor::Transparent;
        }
    }

public:
    ELogVerbosity::Type LogVerbosity;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText LogText;
};

UCLASS()
class DEBUGTOOL_API UDT_LogElement
    : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
    TObjectPtr<UTextBlock> LogText;

};
