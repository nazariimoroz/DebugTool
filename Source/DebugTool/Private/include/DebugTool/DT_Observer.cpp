// FireProject by likapu company


#include "DebugTool/DT_Observer.h"

#include "EngineUtils.h"
#include "DebugTool/DT_Logger.h"

UDT_Observer* UDT_Observer::Singleton = nullptr;

UDT_Observer::UDT_Observer()
{
    DT_RETURN_NO_LOGGER(GEngine);

    OnBeginPieCallbackHandle = FEditorDelegates::BeginPIE.AddRaw(this, &UDT_Observer::OnBeginPieCallback);
    OnWorldAddedCallbackHandle = GEngine->OnWorldAdded().AddRaw(this, &UDT_Observer::OnWorldAddedCallback);
    OnWorldDestroyedCallbackHandle = GEngine->OnWorldDestroyed().AddRaw(this, &UDT_Observer::OnWorldDestroyedCallback);

    FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &UDT_Observer::OnPostWorldInitializationCallback);

    bInited = true;
}

UDT_Observer::~UDT_Observer()
{
    DT_RETURN_NO_LOGGER(GEngine);

    FEditorDelegates::BeginPIE.Remove(OnBeginPieCallbackHandle);
    GEngine->OnWorldAdded().Remove(OnWorldAddedCallbackHandle);
    GEngine->OnWorldDestroyed().Remove(OnWorldDestroyedCallbackHandle);
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

void UDT_Observer::OnPostWorldInitializationCallback(UWorld* World, FWorldInitializationValues WorldInitializationValues)
{
    if(!GEditor->IsPlaySessionInProgress()) return;
    CurrentWorld = World;

    if (CurrentWorld)
    {
        CurrentWorld->AddOnActorSpawnedHandler(
            FOnActorSpawned::FDelegate::CreateRaw(this, &UDT_Observer::OnActorSpawnedCallback));
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

void UDT_Observer::OnActorSpawnedCallback(AActor* Actor)
{
    if (const auto InfoPtr = ObservationInfo.Find(Actor->GetClass()))
    {
        const auto Info = *InfoPtr;
        DT_DISPLAY_NO_LOGGER("Actor spawned {0}", Actor->GetName());
    }
}

void UDT_Observer::OnActorDestroyedCallback(AActor* Actor)
{
}
