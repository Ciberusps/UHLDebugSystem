// Pavel Penkov 2025 All Rights Reserved.


#include "DebugCategories/DCC_AbilitySystem_Effects.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_AbilitySystem_Effects)

void UDCC_AbilitySystem_Effects::Activate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("showdebug abilitysystem"));
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("AbilitySystem.Debug.SetCategory GameplayEffects"));
    return Super::Activate_Implementation(WorldContextObject, PlayerController);
}

void UDCC_AbilitySystem_Effects::Deactivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("showdebug reset"));
    Super::Deactivate_Implementation(WorldContextObject, PlayerController);
}
