// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DT_LoggerFunctionLibrary.generated.h"

class UDT_LogElementInfo;
class UDT_Logger;

UCLASS()
class DEBUGTOOL_API UDT_LoggerFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Logger")
    static TArray<UDT_LogElementInfo*> GetLastLogsInGame(int32 Count = -1);
};
