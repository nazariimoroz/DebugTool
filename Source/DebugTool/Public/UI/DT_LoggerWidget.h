// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "DT_LoggerWidget.generated.h"

class UDT_LogElementInfo;
class UCommonListView;

UCLASS()
class DEBUGTOOL_API UDT_LoggerWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;
    virtual void BeginDestroy() override;

public:

    void AddLog(UDT_LogElementInfo* const LogElementInfo);
    void AddLog(ELogVerbosity::Type LogVerbosity, const FText& Message);

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
    TObjectPtr<UCommonListView> LoggerView;

protected:
#pragma region Callbacks
    void AddLogCallback(ELogVerbosity::Type LogVerbosity, FString Message);
#pragma endregion
};
