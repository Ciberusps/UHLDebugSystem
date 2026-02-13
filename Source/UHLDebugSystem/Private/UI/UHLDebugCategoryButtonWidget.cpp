// Pavel Penkov 2025 All Rights Reserved.


#include "UI/UHLDebugCategoryButtonWidget.h"

#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UHLDebugCategory.h"
#include "UHLDebugSystemSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoryButtonWidget)

void UUHLDebugCategoryButtonWidget::SetUp(const FUHLDebugCategory& UHLDebugCategory_In)
{
    UHLDebugCategory = UHLDebugCategory_In;
    TextBlock->SetText(FText::FromString(UHLDebugCategory_In.Name));
    UpdateCheckboxState(UHLDebugCategory_In.GetIsEnabled());
    Button->SetBackgroundColor(UHLDebugCategory.Color);
    
    // Build tooltip with description and tags
    FString TooltipText = UHLDebugCategory_In.Description;
    
    if (UHLDebugCategory_In.Tags.Num() > 0)
    {
        if (!TooltipText.IsEmpty())
        {
            TooltipText += TEXT("\n\nTags:\n");
        }
        else
        {
            TooltipText = TEXT("Tags:\n");
        }
        
        for (const FGameplayTag& Tag : UHLDebugCategory_In.Tags)
        {
            TooltipText += FString::Printf(TEXT("• %s\n"), *Tag.ToString());
        }
    }
    
    if (!TooltipText.IsEmpty())
    {
        Button->SetToolTipText(FText::FromString(TooltipText));
    }
    
    Button->OnClicked.AddUniqueDynamic(this, &UUHLDebugCategoryButtonWidget::OnButtonClicked);
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (GameInstance)
    {
        UUHLDebugSystemSubsystem* UHLDebugSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
        UHLDebugSubsystem->OnDebugCategoryChanged.AddUniqueDynamic(this, &UUHLDebugCategoryButtonWidget::OnDebugCategoryChanged);
    }
}

void UUHLDebugCategoryButtonWidget::UpdateCheckboxState(bool bEnabled_In)
{
    CheckBox->SetCheckedState(bEnabled_In ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

bool UUHLDebugCategoryButtonWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    // more correct way to avoid error. than in original post.
    if(!HasAnyFlags(RF_ClassDefaultObject))
    {
        // root have to be initialized in Initialize function, otherwise it will not work, donno exactly why.
        Button = WidgetTree->ConstructWidget<UButton>();
        WidgetTree->RootWidget = Button;
    }
    return bIsWidgetInitialized;
}

void UUHLDebugCategoryButtonWidget::NativePreConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree) return;

    HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>();
    CheckBox = WidgetTree->ConstructWidget<UCheckBox>();
    TextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    if (!HorizontalBox || !CheckBox || !TextBlock) return;

    FSlateFontInfo FontInfo = TextBlock->GetFont();
    FontInfo.Size = FMath::RoundToInt(FontInfo.Size / 1.5f);
    TextBlock->SetFont(FontInfo);
    TextBlock->SetShadowOffset(FVector2D(1.0f, 1.0f));
    TextBlock->SetShadowColorAndOpacity(FColor::FromHex("#000000FF"));
    TextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox->SetVisibility(ESlateVisibility::HitTestInvisible);
    CheckBox->SetIsEnabled(false);
    FCheckBoxStyle CheckBoxStyle = CheckBox->GetWidgetStyle();
    CheckBoxStyle.CheckedImage.TintColor = FColor::Black;
    CheckBoxStyle.ForegroundColor = FColor::White;
    CheckBoxStyle.CheckedForeground = FColor::White;
    CheckBoxStyle.HoveredForeground = FColor::White;
    CheckBoxStyle.PressedForeground = FColor::White;
    CheckBoxStyle.UndeterminedForeground = FColor::White;
    CheckBoxStyle.CheckedHoveredForeground = FColor::White;
    CheckBoxStyle.CheckedPressedForeground = FColor::White;
    CheckBox->SetWidgetStyle(CheckBoxStyle);
    HorizontalBox->SetVisibility(ESlateVisibility::HitTestInvisible);

    UButtonSlot* HorizontalBoxButtonSlot = Cast<UButtonSlot>(Button->AddChild(HorizontalBox));
    UHorizontalBoxSlot* CheckboxHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(CheckBox));
    UHorizontalBoxSlot* TextBlockHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(HorizontalBox->AddChild(TextBlock));
    if (!HorizontalBoxButtonSlot || !CheckboxHorizontalBoxSlot || !TextBlockHorizontalBoxSlot) return;

    HorizontalBoxButtonSlot->SetHorizontalAlignment(HAlign_Left);
    CheckboxHorizontalBoxSlot->SetPadding(FMargin(0, 0, 10, 0));
    TextBlockHorizontalBoxSlot->SetHorizontalAlignment(HAlign_Left);
    TextBlockHorizontalBoxSlot->SetVerticalAlignment(VAlign_Top);
}

void UUHLDebugCategoryButtonWidget::OnButtonClicked()
{
    if (OnMadeClick.IsBound())
    {
        OnMadeClick.Broadcast(this, UHLDebugCategory.Tags.First());
    }
}

void UUHLDebugCategoryButtonWidget::OnDebugCategoryChanged(FGameplayTag DebugCategoryTag_In, bool bEnabled_In)
{
    if (UHLDebugCategory.Tags.First() == DebugCategoryTag_In)
    {
        UpdateCheckboxState(bEnabled_In);
    }
}
