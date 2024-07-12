// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_LoggerInGameWidget.generated.h"

class UCommonListView;
class UDT_LogElementInfo;
struct FDT_LogElement;

UENUM(BlueprintType)
enum class EDT_LoggerInGameWidgetState : uint8
{
    MainMenu = 0,
    Logger = 1,
    ClassFields = 2,

    None = TNumericLimits<uint8>::Max()
};

UCLASS()
class DEBUGTOOL_API UDT_LoggerInGameWidget : public UUserWidget
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

    UPROPERTY(BlueprintReadWrite)
    EDT_LoggerInGameWidgetState State = EDT_LoggerInGameWidgetState::None;
};
