// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class FDebugToolModule;
struct FDT_ActorInfo;

struct FDT_VariableInfo
{
    TWeakPtr<FDT_ActorInfo> ActorInfo;
    FProperty* Property;
    bool bIsValid;
    TVariant<int32, bool> Value;
};

struct FDT_ActorInfo
{
    AActor* Actor;
    TArray<TSharedPtr<FDT_VariableInfo>> VariablesInfo;
};

struct FDT_ObservationInfo
{
    UClass* ObservationClass;
    TArray<FProperty*> Properties;
    TMap<AActor*, TSharedPtr<FDT_ActorInfo>> CurrentAvailableActors;
};

class DEBUGTOOL_API UDT_Observer final
{
    friend FDebugToolModule;

    DECLARE_MULTICAST_DELEGATE_OneParam(FDT_OnVariableChangedDelegate, TWeakPtr<FDT_VariableInfo>);

#pragma region Singleton
protected:
    UDT_Observer();
    ~UDT_Observer();

public:
    static UDT_Observer* Get()
    {
        if(!Singleton)
            UE_LOG(LogTemp, Error, TEXT("UDT_Observer is not inited"));
        return Singleton;
    }

protected:
    static UDT_Observer* Singleton;
#pragma endregion

public:
    void AddObservationProperty(UClass* ObservationClass, FName PropertyName);

    const TMap<UClass*, TSharedPtr<FDT_ObservationInfo>>& GetObservationInfo() const { return ObservationInfo; }

protected:
    void CleanUp();

public:
#pragma region Delegates
    FDT_OnVariableChangedDelegate OnVariableChangedDelegate;
#pragma endregion

private:
    bool bInited = false;

    UWorld* CurrentWorld = nullptr;
    TMap<UClass*, TSharedPtr<FDT_ObservationInfo>> ObservationInfo;

    FDelegateHandle OnPostWorldInitializationCallbackHandle;
    FDelegateHandle OnPreWorldFinishDestroyCallbackHandle;
    FDelegateHandle OnWorldTickStartCallbackHandle;
    FDelegateHandle OnActorSpawnedCallbackHandle;
    FDelegateHandle OnActorDestroyedCallbackHandle;

private:
#pragma region Callbacks
    void OnPostWorldInitializationCallback(UWorld* World, FWorldInitializationValues WorldInitializationValues);
    void OnPreWorldFinishDestroyCallback(UWorld* World);
    void OnWorldTickStartCallback(UWorld* World, ELevelTick LevelTick, float X);

    void OnActorSpawnedCallback(AActor* Actor);
    void OnActorDestroyedCallback(AActor* Actor);
#pragma endregion

};
