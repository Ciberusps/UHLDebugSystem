// Pavel Penkov 2025 All Rights Reserved.


#include "UI/UHLDebugCategoriesListWidget.h"

#include "Engine/GameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ButtonSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Development/UHLDebugSystemSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UHLDebugCategoryButtonWidget.h"
#include "UHLDebugSystemSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UHLDebugCategoriesListWidget)

bool UUHLDebugCategoriesListWidget::Initialize()
{
    bool bIsWidgetInitialized = Super::Initialize();

    if(!HasAnyFlags(RF_ClassDefaultObject))
    {
        // Create root vertical box to contain filter box, count text, and scroll box
        UVerticalBox* RootVerticalBox = WidgetTree->ConstructWidget<UVerticalBox>();
        WidgetTree->RootWidget = RootVerticalBox;
        
        // Create filter text box
        FilterTextBox = WidgetTree->ConstructWidget<UEditableTextBox>();
        FilterTextBox->SetHintText(FText::FromString(TEXT("Filter by name or tag...")));
        UVerticalBoxSlot* FilterSlot = Cast<UVerticalBoxSlot>(RootVerticalBox->AddChild(FilterTextBox));
        FilterSlot->SetSize(ESlateSizeRule::Fill);
        FilterSlot->SetPadding(FMargin(0, 0, 0, 10));
        FilterTextBox->OnTextChanged.AddUniqueDynamic(this, &UUHLDebugCategoriesListWidget::OnFilterTextChanged);
        
        // Create categories count text
        CategoriesCountText = WidgetTree->ConstructWidget<UTextBlock>();
        CategoriesCountText->SetText(FText::FromString(TEXT("Categories: 0/0")));
        UVerticalBoxSlot* CountSlot = Cast<UVerticalBoxSlot>(RootVerticalBox->AddChild(CategoriesCountText));
        CountSlot->SetSize(ESlateSizeRule::Fill);
        CountSlot->SetPadding(FMargin(0, 0, 0, 10));
        
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
        FString CountString = FString::Printf(TEXT("Categories: %d/%d"), FilteredCount, TotalCount);
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
