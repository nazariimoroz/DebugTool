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

    OnWorldTickStartCallbackHandle =
    FWorldDelegates::OnWorldTickStart.AddRaw(
        this, &UDT_Observer::OnWorldTickStartCallback);

    bInited = true;
}

UDT_Observer::~UDT_Observer()
{
    FWorldDelegates::OnPostWorldInitialization.Remove(OnPostWorldInitializationCallbackHandle);
    FWorldDelegates::OnPreWorldFinishDestroy.Remove(OnPreWorldFinishDestroyCallbackHandle);
    FWorldDelegates::OnWorldTickStart.Remove(OnWorldTickStartCallbackHandle);
}

void UDT_Observer::AddObservationProperty(UClass* ObservationClass, FName PropertyName)
{
    DT_RETURN_NO_LOGGER(bInited);
    DT_RETURN_NO_LOGGER(IsValid(ObservationClass));

    auto InfoPtr = ObservationInfo.Find(ObservationClass);
    if (!InfoPtr)
    {
        auto Ptr = MakeShared<FDT_ObservationInfo>(ObservationClass);
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

void UDT_Observer::OnWorldTickStartCallback(UWorld* World, ELevelTick LevelTick, float X)
{
    if (!GEditor->IsPlaySessionInProgress()) return;

    for (const auto& [_, ClassInfo] : ObservationInfo)
    {
        if(!ClassInfo.IsValid())
            continue;

        for (const auto& [__, ObjInfo] : ClassInfo->CurrentAvailableActors)
        {
            if(!ObjInfo.IsValid())
                continue;

            const auto* const Actor = ObjInfo->Actor;

            for (const auto& VarInfo : ObjInfo->VariablesInfo)
            {
                if(!VarInfo.IsValid() || !VarInfo->bIsValid)
                    continue;

                if(const auto IntProp = CastField<FIntProperty>(VarInfo->Property))
                {
                    const auto Value = IntProp->ContainerPtrToValuePtr<int32>(Actor);
                    if(!Value)
                    {
                        DT_ERROR_NO_LOGGER("Bad variable: {0}", IntProp->GetName());
                        continue;
                    }

                    if(const auto PrevValue = VarInfo->Value.TryGet<int32>())
                    {
                        if(*PrevValue != *Value)
                        {
                            VarInfo->Value.Set<int32>(*Value);
                            OnVariableChangedDelegate.Broadcast(VarInfo.ToWeakPtr());
                        }
                    }
                    else
                    {
                        DT_ERROR_NO_LOGGER("Bad variant value: {0}", IntProp->GetName());
                        VarInfo->Value.Set<int32>(*Value);
                        OnVariableChangedDelegate.Broadcast(VarInfo.ToWeakPtr());
                    }
                }
            }
        }
    }
}

void UDT_Observer::OnActorSpawnedCallback(AActor* Actor)
{
    if(!IsValid(Actor))
        return;

    if (const auto ClassInfoPtr = ObservationInfo.Find(Actor->GetClass()))
    {
        const auto ClassInfo = *ClassInfoPtr;
        if(ClassInfo->CurrentAvailableActors.Find(Actor))
            return;

        const auto ActorInfo = MakeShared<FDT_ActorInfo>(Actor);
        for (auto Property : ClassInfo->Properties)
        {
            TSharedPtr<FDT_VariableInfo> VariableInfo = nullptr;

            if(const auto IntProp = CastField<FIntProperty>(Property))
            {
                const auto Value = IntProp->ContainerPtrToValuePtr<int32>(Actor);

                VariableInfo = MakeShared<FDT_VariableInfo>(ActorInfo.ToWeakPtr());
                if(!Value)
                {
                    VariableInfo->Property = Property;
                    VariableInfo->bIsValid = false;
                    continue;
                }

                VariableInfo->Property = Property;
                VariableInfo->bIsValid = true;
                VariableInfo->Value.Set<int32>(*Value);
            }

            if(const auto BoolProp = CastField<FBoolProperty>(Property))
            {
                const auto Value = BoolProp->ContainerPtrToValuePtr<bool>(Actor);

                VariableInfo = MakeShared<FDT_VariableInfo>(ActorInfo.ToWeakPtr());
                if(!Value)
                {
                    VariableInfo->Property = Property;
                    VariableInfo->bIsValid = false;
                    continue;
                }

                VariableInfo->Property = Property;
                VariableInfo->bIsValid = true;
                VariableInfo->Value.Set<bool>(*Value);
            }

            ActorInfo->VariablesInfo.Add(VariableInfo);
            OnVariableChangedDelegate.Broadcast(VariableInfo.ToWeakPtr());
        }

        ClassInfo->CurrentAvailableActors.Add(MakeTuple(Actor, ActorInfo));
    }
}

void UDT_Observer::OnActorDestroyedCallback(AActor* Actor)
{
    if(!IsValid(Actor))
        return;

    if (const auto ClassInfoPtr = ObservationInfo.Find(Actor->GetClass()))
    {
        const auto ClassInfo = *ClassInfoPtr;

        ClassInfo->CurrentAvailableActors.Remove(Actor);
    }
}
