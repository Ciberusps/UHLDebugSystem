// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugCategoryComponent.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoryComponent)

APlayerController* UUHLDebugCategoryComponent::GetPlayerControllerFromContext(UObject* WorldContextObject, int32 PlayerIndex) const
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    if (UWorld* World = WorldContextObject->GetWorld())
    {
        return UGameplayStatics::GetPlayerController(World, PlayerIndex);
    }

    return nullptr;
}

bool UUHLDebugCategoryComponent::CanActivate_Implementation(UObject* WorldContextObject) const
{
    return true;
}

void UUHLDebugCategoryComponent::Activate_Implementation(UObject* WorldContextObject)
{
}

void UUHLDebugCategoryComponent::Deactivate_Implementation(UObject* WorldContextObject)
{
}
