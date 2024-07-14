// FireProject by likapu company


#include "DebugTool/DT_Observer.h"

#include "DebugTool/DT_Logger.h"

UDT_Observer* UDT_Observer::Singleton = nullptr;

UDT_Observer::UDT_Observer()
{
    UE_LOG(LogTemp, Warning, TEXT("Inited!!!"));

    OnWorldAddedCallbackHandle = FWorldDelegates::OnWorldTickStart.AddRaw(this, &UDT_Observer::OnWorldAddedCallback);
    OnWorldDestroyedCallbackHandle = FWorldDelegates::OnWorldTickEnd.AddRaw(this, &UDT_Observer::OnWorldDestroyedCallback);

    bInited = true;
}

UDT_Observer::~UDT_Observer()
{
    FWorldDelegates::OnWorldTickStart.Remove(OnWorldAddedCallbackHandle);
    FWorldDelegates::OnWorldTickEnd.Remove(OnWorldDestroyedCallbackHandle);
}

void UDT_Observer::AddObservationProperty(UClass* ObservationClass, FName PropertyName)
{
    DT_RETURN_NO_LOGGER(bInited);
    DT_RETURN_NO_LOGGER(IsValid(ObservationClass));

    auto InfoPtr = ObservationInfo.Find(ObservationClass);
    if (!InfoPtr)
    {
        auto Ptr = MakeShared<FObservationInfo>(ObservationClass);
        InfoPtr = &ObservationInfo.Add(MakeTuple(ObservationClass, MoveTempIfPossible(Ptr)));
        DT_RETURN_NO_LOGGER(InfoPtr);
    }

    const auto Info = InfoPtr->Get();
    DT_RETURN_NO_LOGGER(Info);

    auto Property = ObservationClass->FindPropertyByName(PropertyName);
    DT_RETURN_NO_LOGGER(Property);

    if (Info->Properties.Contains(Property))
        return;

    Info->Properties.Add(Property);
}

void UDT_Observer::OnWorldAddedCallback(UWorld* World, ELevelTick LevelTick, float X)
{
    UE_LOG(LogTemp, Warning, TEXT("Created!!!"));
    CurrentWorld = World;
}

void UDT_Observer::OnWorldDestroyedCallback(UWorld* World, ELevelTick LevelTick, float X)
{
    UE_LOG(LogTemp, Warning, TEXT("Destroyed!!!"));
    if (CurrentWorld == World) CurrentWorld = nullptr;
}
