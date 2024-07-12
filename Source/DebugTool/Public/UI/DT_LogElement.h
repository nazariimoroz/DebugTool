// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_LogElement.generated.h"

class UTextBlock;

UCLASS()
class DEBUGTOOL_API UDT_LogElement
    : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
    TObjectPtr<UTextBlock> LogText;

};
