// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DT_LogElementInfo.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UDT_LogElementInfo : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FLinearColor LogVerbosityColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText LogText;
};

