// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DT_LogElementInfo.generated.h"

struct FDT_LogElement;

UCLASS(Blueprintable, BlueprintType)
class DEBUGTOOL_API UDT_LogElementInfo : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FLinearColor LogVerbosityColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText LogText;

    static UDT_LogElementInfo* Create(const FDT_LogElement& LogElement);
};

