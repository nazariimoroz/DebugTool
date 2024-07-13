// FireProject by likapu company

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class FDebugToolModule;

struct FObservationInfo
{
    UClass* ObservationClass;
    TArray<FProperty*> Properties;
    TSet<AActor*> CurrentAvailableActors;
};

class DEBUGTOOL_API UDT_Observer final
{
    friend FDebugToolModule;

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

private:
    bool bInited = false;

    UWorld* CurrentWorld = nullptr;
    TMap<UClass*, TSharedPtr<FObservationInfo>> ObservationInfo;

    FDelegateHandle OnWorldAddedCallbackHandle;
    FDelegateHandle OnWorldDestroyedCallbackHandle;

private:
#pragma region Callbacks
    void OnWorldAddedCallback(UWorld* World);
    void OnWorldDestroyedCallback(UWorld* World);
#pragma endregion

};
