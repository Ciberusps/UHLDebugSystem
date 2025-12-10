// Pavel Penkov 2025 All Rights Reserved.


#include "DebugCategories/DCC_Collisions.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_Collisions)

void UDCC_Collisions::Activate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
	UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("show Collision"));
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("ShowFlag.Collision 1"));
    Super::Activate_Implementation(WorldContextObject, PlayerController);
}

void UDCC_Collisions::Deactivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("ShowFlag.Collision 0"));
	UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("show Collision"));
    Super::Deactivate_Implementation(WorldContextObject, PlayerController);
}
