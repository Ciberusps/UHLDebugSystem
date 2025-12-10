// Pavel Penkov 2025 All Rights Reserved.

#include "UHLDebugCategory.h"

#include "Templates/SubclassOf.h"
#include "Kismet/GameplayStatics.h"
#include "UHLDebugCategoryComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategory)

bool FUHLDebugCategory::TryEnable(UObject* ContextObj)
{
    // Need a valid world-aware context for activation
    if (!ContextObj || !ContextObj->GetWorld())
    {
        return false;
    }

    bool bResult = false;

    // Resolve controller once per activation call
    APlayerController* PlayerController = nullptr;
    if (UWorld* World = ContextObj->GetWorld())
    {
        PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    }

    int32 ComponentsActivated = 0;
    int32 ComponentsRequiredToActivate = Components.Num();
    for (TSubclassOf<UUHLDebugCategoryComponent> ComponentClass : Components)
    {
        if (!ComponentClass)
        {
            ComponentsRequiredToActivate--;
            continue;
        };

        UUHLDebugCategoryComponent* Component = GetOrCreateDebugCategoryComponent(ComponentClass, ContextObj);
        if (Component->CanActivate(ContextObj, PlayerController))
        {
            Component->Activate(ContextObj, PlayerController);
            ComponentsActivated++;
        }
    }
    if (ComponentsActivated == ComponentsRequiredToActivate)
    {
        bResult = true;
    }

    bIsEnabled = true;
    return bResult;
}

void FUHLDebugCategory::TryDisable(UObject* ContextObj)
{
    APlayerController* PlayerController = nullptr;
    if (ContextObj && ContextObj->GetWorld())
    {
        PlayerController = UGameplayStatics::GetPlayerController(ContextObj->GetWorld(), 0);
    }

    for (UUHLDebugCategoryComponent* InstancedComponent : InstancedComponents)
    {
        if (InstancedComponent)
        {
            InstancedComponent->Deactivate(ContextObj, PlayerController);
        }
    }
    bIsEnabled = false;
}

UUHLDebugCategoryComponent* FUHLDebugCategory::GetOrCreateDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj)
{
    UUHLDebugCategoryComponent* Component = nullptr;
    UUHLDebugCategoryComponent** FoundComponent = GetDebugCategoryComponent(ComponentClass, ContextObj);
    if (!FoundComponent)
    {
        Component = NewObject<UUHLDebugCategoryComponent>(ContextObj, ComponentClass);
        InstancedComponents.Add(Component);
    }
    else
    {
        Component = *FoundComponent;
    }

    return Component;
}

UUHLDebugCategoryComponent** FUHLDebugCategory::GetDebugCategoryComponent(TSubclassOf<UUHLDebugCategoryComponent> ComponentClass, UObject* ContextObj)
{
    return InstancedComponents.FindByPredicate([=](UUHLDebugCategoryComponent* DebugCategoryComponent)
        {
            return DebugCategoryComponent->GetClass() == ComponentClass;
        });
}
