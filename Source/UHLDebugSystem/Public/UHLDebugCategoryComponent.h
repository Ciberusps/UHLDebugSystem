// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UHLDebugCategoryComponent.generated.h"

// TODO force activation with "MMB" click
UCLASS(Abstract, BlueprintType, Blueprintable)
class UHLDEBUGSYSTEM_API UUHLDebugCategoryComponent : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "PlayerController"))
    void Activate(UObject* WorldContextObject, APlayerController* PlayerController = nullptr);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "PlayerController"))
    void Deactivate(UObject* WorldContextObject, APlayerController* PlayerController = nullptr);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UHLDebugSubsystem", meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "PlayerController"))
    bool CanActivate(UObject* WorldContextObject, APlayerController* PlayerController = nullptr) const;
};
