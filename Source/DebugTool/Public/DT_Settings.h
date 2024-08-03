// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DT_Settings.generated.h"

UCLASS(config = Editor, defaultconfig)
class DEBUGTOOL_API UDT_Settings : public UObject
{
    GENERATED_BODY()

public:
    UDT_Settings(const FObjectInitializer& Initializer);

    UPROPERTY(Config, EditAnywhere, Category = "Debug Tool")
    bool bEnableLogFile;

    /**
     * If LogFilePath is Empty, ProjectLogDir is used
     */
    UPROPERTY(Config, EditAnywhere, Category = "Debug Tool", meta = (EditCondition = "bEnableLogFile", EditConditionHides = true))
    FString LogFilePath;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
