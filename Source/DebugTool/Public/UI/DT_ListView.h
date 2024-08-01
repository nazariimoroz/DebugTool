// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "DT_ListView.generated.h"

UCLASS()
class DEBUGTOOL_API UDT_ListView : public UCommonListView
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void AddItemAt(UObject* Item, int32 Index);
};
