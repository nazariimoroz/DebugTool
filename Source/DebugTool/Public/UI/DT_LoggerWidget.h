// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "DT_LoggerWidget.generated.h"

class UDT_ListView;
struct FDT_LogElement;
class UDT_LogElementInfo;

UCLASS()
class DEBUGTOOL_API UDT_LoggerWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool Refresh();

public:
    void AddLog(UDT_LogElementInfo* const LogElementInfo);
    void AddLog(const FDT_LogElement& LogElement);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
    TObjectPtr<UDT_ListView> LoggerView;
};
