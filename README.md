# 🪲 UHLDebugSystem

**UHLDebugSystem** - gameplay tags based debug system for runtime. Any game needs debug system, in mid-size commands you always use limited set of debugging tools
more always than others, so **DebugSystem** is as tool for creating your debug system as fast as possible by providing `DebugCategories`(gameplaytags) that can be enabled/disabled. 
In mid-size commands you always use limited set of debugging tools, UHLDebugSystem perfect tool to unify their debugging.

Use case:
I want to have debug for AbilitySystem, it should turn on/off, available in editor between sessions and.....

Part of [UnrealHelperLibrary](https://github.com/Ciberusps/unreal-helper-library).

Support: `UE5.4(at least) - UE5.6`

TODO: screenshot

## ✨ Features

- Define DebugCategories using tags, Block other DebugCategories by tags
- WaitDebugCategory async action for BPs with "Check on Start" option that checks is DebugCategory enabled on start, no need to write BP spaggety for debug
- DebugCategoriesSubsystem - for easy access to DebugCategories
- Create DebugCategoriesComponents with Blueprint logic and do something special on Activation/Deactivation of DebugCategory(like GAS abilities) - for example enable Console Vars
- You can even compose DebugCategoryComponents e.g. you want Collisions + HitBoxes, so you can create combined DebugCategory and add "DCC_Collisions" and "DCC_HitBoxes"
- Branch your logic depending on DebugCategories - for your demo/release game version

## 🚀 Install & ⬆️ Update

### From github (recommended)

```bash
// install as git submodule to your plugins folder
git submodule add https://github.com/Ciberusps/UHLDebugSystem.git ./Plugins/UHLDebugSystem

// to update plugin run
git submodule update --remote
```

</details>

### From marketplace

for now not listed

## 💡Use

> - [Subscibe on DebugCategory change]()
> - [Add DebugCategory]()
> - [Integrate in DebugMenu]()

### Add DebugCategory

1) Open `Project Settings` -> in UnrealHelperLibrary category look for `UHLDebugSystem`
2) Add your first debug category

### Subscibe on DebugCategory change

in blueprints

- `WaitDebugCategoryChange`
- `UHLDebugSystemSubsystem`

in C++

```c++
    // with AsyncAction - WaitDebugCategoryChange
    UAA_WaitDebugCategoryChange* WaitDebugCategoryChangeTask = UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(
        Actor->GetWorld(),
        FGameplayTag("DebugCategory.Something")
    );
    WaitDebugCategoryChangeTask->OnChange.AddUniqueDynamic(this, &ThisClass::OnDebugCategoryChanged);
    WaitDebugCategoryChangeTask->Activate();  // on activation "OnDebugCategoryChanged" will be fired

    // or with subsystem
    UUHLDebugSystemSubsystem* UHLDebugSystemSubsystem = GameInstance->GetSubsystem<UUHLDebugSystemSubsystem>();
    UHLDebugSystemSubsystem->OnDebugCategoryChanged.AddUniqueDynamic(this, &ThisClass::OnDebugCategoryChange);
```

### Integrate in DebugMenu

- Add `UHLDebugCategoriesListWidget` to your DebugMenu or just run `UHLDebugSystem.ShowCategories 1` for widget with DebugCategories shown.

## 📄 Documentation

> - Subsystem
>   - [UHLDebugSystemSubsystem](#uhldebugsystemsubsystem)
> - AsyncActions
>   - [WaitDebugCategoryChange](#aa_waitdebugcategorychange)
> - [BlueprintLibrary](#debugblueprintlibrary)
>   - [IsUHLDebugCategoryEnabled](#isuhldebugcategoryenabled)
> - Settings
>   - [DebugSystemSettings](#debugsystemsettings)
> - Components
>   - [DebugCategoryComponent]()

### DebugSystemSettings

- add new categories, turn on/off default state, every debug category is a tag
- `DebugCategoryComponents` (DCC)

### Check is category enabled/subscribe on debug category state change

- `WaitDebugCategoryChange` - blueprint node to easier check `isDebugCategoryEnabled` or not and wait for its changes
- `IsUHLDebugSystemEnabled` - blueprint node
- `DebugCategoriesList` - UI component for quick integration in your debug menu

### DebugSystemSubsystem

### `UHLDebugSystemSubsystem`

**UHLDebugSystemSubsystem** - subsystem that provides interaction with debug categories

### BlueprintLibrary

#### `IsUHLDebugCategoryEnabled`

Check is debug category enabled or not
