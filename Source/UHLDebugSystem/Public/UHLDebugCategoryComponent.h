// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UHLDebugCategoryComponent.generated.h"

// TODO force activation with "MMB" click
UCLASS(Abstract, Blueprintable)
class UHLDEBUGSYSTEM_API UUHLDebugCategoryComponent : public UObject
{
    GENERATED_BODY()

public:
    // Helper to resolve a player controller from any world-aware context object.
    UFUNCTION(BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject"))
    APlayerController* GetPlayerControllerFromContext(UObject* WorldContextObject, int32 PlayerIndex = 0) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject"))
    void Activate(UObject* WorldContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject"))
    void Deactivate(UObject* WorldContextObject);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject"))
    bool CanActivate(UObject* WorldContextObject) const;
};
