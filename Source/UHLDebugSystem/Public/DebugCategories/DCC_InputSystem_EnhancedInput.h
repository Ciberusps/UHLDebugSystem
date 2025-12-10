// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UHLDebugCategoryComponent.h"
#include "DCC_InputSystem_EnhancedInput.generated.h"

/**
 *
 */
UCLASS()
class UHLDEBUGSYSTEM_API UDCC_InputSystem_EnhancedInput : public UUHLDebugCategoryComponent
{
	GENERATED_BODY()

public:
    virtual void Activate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController = nullptr) override;
    virtual void Deactivate_Implementation(UObject* WorldContextObject, APlayerController* PlayerController = nullptr) override;
};
