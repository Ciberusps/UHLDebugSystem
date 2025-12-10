// Pavel Penkov 2025 All Rights Reserved.


#include "DebugCategories/DCC_InputSystem_EnhancedInput.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DCC_InputSystem_EnhancedInput)

void UDCC_InputSystem_EnhancedInput::Activate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("showdebug enhancedinput"));
    Super::Activate_Implementation(WorldContextObject, PlayerController);
}

void UDCC_InputSystem_EnhancedInput::Deactivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController)
{
    UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject->GetWorld(), FString("showdebug reset"));
    Super::Deactivate_Implementation(WorldContextObject, PlayerController);
}
