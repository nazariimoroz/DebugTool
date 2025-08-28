// FireProject by likapu company


#include "DT_Settings.h"

#include "DebugTool/DT_LoggerSubsystem.h"

UDT_Settings::UDT_Settings(const FObjectInitializer& Initializer)
    : Super(Initializer)
{
}

#if WITH_EDITOR
void UDT_Settings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    UObject::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
