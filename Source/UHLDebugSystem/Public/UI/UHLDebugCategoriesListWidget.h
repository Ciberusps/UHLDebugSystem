// Pavel Penkov 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UHLDebugCategory.h"
#include "UHLDebugCategoriesListWidget.generated.h"

class UUHLDebugSystemSubsystem;
class UUHLDebugCategoryButtonWidget;
class UCanvasPanel;
/**
 *
 */
UCLASS()
class UHLDEBUGSYSTEM_API UUHLDebugCategoriesListWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

private:
    UPROPERTY()
    UScrollBox* ScrollBox;
    
    UPROPERTY()
    UEditableTextBox* FilterTextBox;
    
    UPROPERTY()
    UTextBlock* CategoriesCountText;
    
    TWeakObjectPtr<UUHLDebugSystemSubsystem> UHLDebugSubsystem;
    
    FString FilterText;
    
    UFUNCTION()
    void OnButtonClicked(UUHLDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag);
    
    UFUNCTION()
    void OnFilterTextChanged(const FText& InText);
    
    void RefreshCategoryList();
    
    void UpdateCategoriesCount(int32 TotalCount, int32 FilteredCount);
    
    bool MatchesFilter(const FUHLDebugCategory& Category) const;
};
