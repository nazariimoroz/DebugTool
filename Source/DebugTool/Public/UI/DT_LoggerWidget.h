// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "DT_LoggerWidget.generated.h"

struct FDT_LogElement;
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
    void AddLog(const FDT_LogElement& LogElement);

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
    TObjectPtr<UCommonListView> LoggerView;
};
