// FireProject by likapu company


#include "DebugTool/DT_Observer.h"

#include "EngineUtils.h"
#include "DebugTool/DT_Logger.h"

UDT_Observer* UDT_Observer::Singleton = nullptr;

UDT_Observer::UDT_Observer()
{
    OnPostWorldInitializationCallbackHandle
        = FWorldDelegates::OnPostWorldInitialization.AddRaw(
            this, &UDT_Observer::OnPostWorldInitializationCallback);

    OnPreWorldFinishDestroyCallbackHandle =
    FWorldDelegates::OnPreWorldFinishDestroy.AddRaw(
            this, &UDT_Observer::OnPreWorldFinishDestroyCallback);

    bInited = true;
}

UDT_Observer::~UDT_Observer()
{
    FWorldDelegates::OnPostWorldInitialization.Remove(OnPostWorldInitializationCallbackHandle);
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

void UDT_Observer::CleanUp()
{
    if (CurrentWorld)
    {
        if (OnActorSpawnedCallbackHandle.IsValid())
            CurrentWorld->RemoveOnActorSpawnedHandler(OnActorSpawnedCallbackHandle);

        if (OnActorDestroyedCallbackHandle.IsValid())
            CurrentWorld->RemoveOnActorDestroyededHandler(OnActorDestroyedCallbackHandle);

        CurrentWorld = nullptr;
    }

    for (auto& [ObsClass, Info] : ObservationInfo)
    {
        Info->CurrentAvailableActors.Reset();
    }
}

void UDT_Observer::OnPostWorldInitializationCallback(UWorld* World, FWorldInitializationValues WorldInitializationValues)
{
    if (!GEditor->IsPlaySessionInProgress()) return;

    if(CurrentWorld)
        CleanUp();

    CurrentWorld = World;

    if (CurrentWorld)
    {
        OnActorSpawnedCallbackHandle =
            CurrentWorld->AddOnActorSpawnedHandler(
                FOnActorSpawned::FDelegate::CreateRaw(this, &UDT_Observer::OnActorSpawnedCallback));

        OnActorDestroyedCallbackHandle =
            CurrentWorld->AddOnActorDestroyedHandler(
                FOnActorSpawned::FDelegate::CreateRaw(this, &UDT_Observer::OnActorDestroyedCallback));

        if (GEditor->IsPlaySessionInProgress())
        {
            for (TActorIterator<AActor> ActorItr(CurrentWorld); ActorItr; ++ActorItr)
            {
                OnActorSpawnedCallback(*ActorItr);
            }
        }
    }
}

void UDT_Observer::OnPreWorldFinishDestroyCallback(UWorld* World)
{
    if(CurrentWorld == World)
        CleanUp();
}

void UDT_Observer::OnActorSpawnedCallback(AActor* Actor)
{
    if (const auto InfoPtr = ObservationInfo.Find(Actor->GetClass()))
    {
        const auto Info = *InfoPtr;
        for (auto Property : Info->Properties)
        {
            if(const auto IntProp = CastField<FIntProperty>(Property))
            {
                const auto Value = IntProp->ContainerPtrToValuePtr<int32>(Actor);
                DT_ERROR_NO_LOGGER("Value {0}", *Value);
            }
        }
    }
}

void UDT_Observer::OnActorDestroyedCallback(AActor* Actor)
{
}
