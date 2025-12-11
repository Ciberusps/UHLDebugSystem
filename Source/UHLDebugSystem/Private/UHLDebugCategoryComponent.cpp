// Pavel Penkov 2025 All Rights Reserved.


#include "UHLDebugCategoryComponent.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoryComponent)

bool UUHLDebugCategoryComponent::CanActivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController) const
{
    return true;
}

void UUHLDebugCategoryComponent::Activate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
}

void UUHLDebugCategoryComponent::Deactivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
}
