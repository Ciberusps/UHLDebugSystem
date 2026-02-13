// Pavel Penkov 2025 All Rights Reserved.


#include "UI/UHLDebugCategoriesListWidget.h"

#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ButtonSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Development/UHLDebugSystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugCategoryButtonWidget.h"
#include "UHLDebugSystemSubsystem.h"
#include "Components/SizeBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoriesListWidget)

bool UUHLDebugCategoriesListWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    if(!HasAnyFlags(RF_ClassDefaultObject))
    {
        // Create root vertical box to contain top bar and scroll box
        UVerticalBox* RootVerticalBox = WidgetTree->ConstructWidget<UVerticalBox>();
        WidgetTree->RootWidget = RootVerticalBox;
        
        // Create horizontal box for filter and count
        UHorizontalBox* TopBarHorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>();

		// Create size-limited container for top bar
		USizeBox* TopBarSizeBox = WidgetTree->ConstructWidget<USizeBox>();
		TopBarSizeBox->SetMaxDesiredHeight(30.0f);
		TopBarSizeBox->AddChild(TopBarHorizontalBox);
		
		UVerticalBoxSlot* TopBarSlot = Cast<UVerticalBoxSlot>(RootVerticalBox->AddChild(TopBarSizeBox));
		TopBarSlot->SetSize(ESlateSizeRule::Automatic);
		TopBarSlot->SetPadding(FMargin(0, 0, 0, 10));
        
        // Create filter text box
        FilterTextBox = WidgetTree->ConstructWidget<UEditableTextBox>();
        FilterTextBox->SetHintText(FText::FromString(TEXT("Filter by name/tag")));
        FSlateFontInfo FilterTextBoxFontInfo = FilterTextBox->WidgetStyle.TextStyle.Font;
        FilterTextBoxFontInfo.Size = 14;
    	FilterTextBox->WidgetStyle.BackgroundColor = FLinearColor(0.13f, 0.14f, 0.15f, 1.0f);
    	FilterTextBox->WidgetStyle.TextStyle.ColorAndOpacity = FSlateColor(FLinearColor::White);
        FilterTextBox->WidgetStyle.TextStyle.SetFont(FilterTextBoxFontInfo);
        UHorizontalBoxSlot* FilterSlot = Cast<UHorizontalBoxSlot>(TopBarHorizontalBox->AddChild(FilterTextBox));
        FilterSlot->SetSize(ESlateSizeRule::Fill);
        FilterSlot->SetPadding(FMargin(0, 0, 10, 0));
        FilterTextBox->OnTextChanged.AddUniqueDynamic(this, &UUHLDebugCategoriesListWidget::OnFilterTextChanged);
        
        // Create categories count text
        CategoriesCountText = WidgetTree->ConstructWidget<UTextBlock>();
        CategoriesCountText->SetText(FText::FromString(TEXT("0/0")));
        CategoriesCountText->SetMinDesiredWidth(30.0f);
    	FSlateFontInfo CategoriesCountTextFontInfo = CategoriesCountText->GetFont();
    	CategoriesCountTextFontInfo.Size = FMath::RoundToInt(CategoriesCountTextFontInfo.Size / 1.2f);
    	CategoriesCountText->SetFont(CategoriesCountTextFontInfo);
    	CategoriesCountText->SetShadowOffset(FVector2D(1.0f, 1.0f));
    	CategoriesCountText->SetShadowColorAndOpacity(FColor::FromHex("#000000FF"));
        UHorizontalBoxSlot* CountSlot = Cast<UHorizontalBoxSlot>(TopBarHorizontalBox->AddChild(CategoriesCountText));
        CountSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
        
        // Create scroll box for categories
        ScrollBox = WidgetTree->ConstructWidget<UScrollBox>();
        ScrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::Always);
        ScrollBox->SetVisibility(ESlateVisibility::Visible);
        UVerticalBoxSlot* ScrollSlot = Cast<UVerticalBoxSlot>(RootVerticalBox->AddChild(ScrollBox));
        ScrollSlot->SetSize(ESlateSizeRule::Fill);
    }

    return bIsWidgetInitialized;
}

void UUHLDebugCategoriesListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UHLDebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUHLDebugSystemSubsystem>();

    RefreshCategoryList();
}

void UUHLDebugCategoriesListWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    const UUHLDebugSystemSettings* DeveloperSettings = GetDefault<UUHLDebugSystemSettings>();
    for (TTuple<FGameplayTag, bool> EnabledDebugCategory : DeveloperSettings->EnabledDebugCategories)
    {
        UUHLDebugCategoryButtonWidget* UHLDebugCategoryButton = WidgetTree->ConstructWidget<UUHLDebugCategoryButtonWidget>();
        UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(UHLDebugCategoryButton));

        const FUHLDebugCategory* UHLDebugCategory = DeveloperSettings->DebugCategories.FindByPredicate([=](const FUHLDebugCategory& DebugCategory)
		{
			return DebugCategory.Tags.HasAnyExact(FGameplayTagContainer(EnabledDebugCategory.Key));
		});
        if (UHLDebugCategory != nullptr)
        {
			UHLDebugCategoryButton->SetUp(*UHLDebugCategory);
			ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 10));
        }
    }
}

void UUHLDebugCategoriesListWidget::OnButtonClicked(UUHLDebugCategoryButtonWidget* Button, FGameplayTag DebugCategoryGameplayTag)
{
    if (!Button) return;

    UHLDebugSubsystem->ToggleDebugCategory(DebugCategoryGameplayTag);
    Button->UpdateCheckboxState(UHLDebugSubsystem->IsCategoryEnabled(DebugCategoryGameplayTag));
}

void UUHLDebugCategoriesListWidget::OnFilterTextChanged(const FText& InText)
{
    FilterText = InText.ToString();
    RefreshCategoryList();
}

void UUHLDebugCategoriesListWidget::RefreshCategoryList()
{
    if (!ScrollBox || !UHLDebugSubsystem.IsValid())
    {
        return;
    }

    ScrollBox->ClearChildren();

    const TArray<FUHLDebugCategory>& AllDebugCategories = UHLDebugSubsystem->GetDebugCategories();
    int32 FilteredCount = 0;

    for (const FUHLDebugCategory& UHLDebugCategory : AllDebugCategories)
    {
        if (MatchesFilter(UHLDebugCategory))
        {
            UUHLDebugCategoryButtonWidget* UHLDebugCategoryButton = WidgetTree->ConstructWidget<UUHLDebugCategoryButtonWidget>();
            UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(UHLDebugCategoryButton));
            UHLDebugCategoryButton->SetUp(UHLDebugCategory);
            UHLDebugCategoryButton->OnMadeClick.AddUniqueDynamic(this, &UUHLDebugCategoriesListWidget::OnButtonClicked);

            ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 10));
            ScrollBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            
            FilteredCount++;
        }
    }

    UpdateCategoriesCount(AllDebugCategories.Num(), FilteredCount);
}

void UUHLDebugCategoriesListWidget::UpdateCategoriesCount(int32 TotalCount, int32 FilteredCount)
{
    if (CategoriesCountText)
    {
        FString CountString = FString::Printf(TEXT("%d/%d"), FilteredCount, TotalCount);
        CategoriesCountText->SetText(FText::FromString(CountString));
    }
}

bool UUHLDebugCategoriesListWidget::MatchesFilter(const FUHLDebugCategory& Category) const
{
    // If filter is empty, show all categories
    if (FilterText.IsEmpty())
    {
        return true;
    }

    FString FilterLower = FilterText.ToLower();

    // Check if category name matches
    if (Category.Name.ToLower().Contains(FilterLower))
    {
        return true;
    }

    // Check if category description matches
    if (Category.Description.ToLower().Contains(FilterLower))
    {
        return true;
    }

    // Check if any tag matches
    for (const FGameplayTag& Tag : Category.Tags)
    {
        if (Tag.ToString().ToLower().Contains(FilterLower))
        {
            return true;
        }
    }

    return false;
}
