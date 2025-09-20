# UHLDebugSystem

**UHL** - unreal helper library, toolset to help developers working with AI, GAS, customizing editor and so on.
Goal is to became a tool that insta-installed on new project creation.
All tools are mostly tested on melee combat so if you have other background and think that something should
work another way or have an idea on how to improve developer experience feel free to [discuss](https://github.com/Ciberusps/unreal-helper-library/discussions).

**GAS things not required to be used at all, you can use library only for AI things**. GAS features provides
much smoother GAS experience mostly based on [Lyra](https://dev.epicgames.com/documentation/en-us/unreal-engine/lyra-sample-game-in-unreal-engine?application_version=5.5) features.
All GAS features designed in mind that they or their part can be added or dropped by you in development in any time
and replaced by something custom that fits your project needs

Support: TBD at least UE5.6

![AiModule](https://github.com/user-attachments/assets/4becb592-c02e-423a-bf80-fcfc629ce518)

## ✨ Features

- AI nodes
  - `GameplayFocus` - set focus on actor
  - `RandomChance` - randomize actions
  - `InRange` - check distance to enemy
  - `InAngle` - check angle to enemy
  - `PlayAnimMontage` - play attack animation
  - `SetBBValue` - set blackboard value
  - `DebugPrintBBValue` - print blackboard value
  - `DebugPrintString` - print string on screen
  - `InvokeGameplayAbility` - activate/deactivate GAS Gameplay Ability by tag, with optional "wait for finishing"
  - `TurnTo` - turn enemy using turn animations
- GAS - Lyra based inputs, ability system component, ability system config, input cache, attribute set, ability sets
- **GAS** strong Lyra based toolset for quick start with GAS and scalable for big projects
- GAS abilities input binding using tags based on Lyra and enhanced with real project needs
- GameplayTags based **DebugSystem**
- Utils - utility functions for your game, e.g. `GetProjectVersion`, `GetAssetsOfClass`, `GetHighestPoint`
- Editor - editor customization, e.g. `Custom thumnails`, `Custom class icon`

## 🚀 Install

#### From source (recommended):

- add git submodule to your plugins folder

    ```bash
    git submodule add https://github.com/Ciberusps/UHLDebugSystem.git ./Plugins/UHLDebugSystem
    ```

- <details>
  <summary>C++ setup, old engine versions</summary>

  - to use C++ things add code to file `<ProjectName>.Build.cs`

  ```C#
      // <ProjectName>.Build.cs
      public GameName(ReadOnlyTargetRules Target) : base(Target)
      {
          PublicDependencyModuleNames.AddRange(new string[] {
              // add "UnrealHelperLibrary" to use it in C++
              "UnrealHelperLibrary",
          });

          // OPTIONALLY add "UnrealHelperEditor" module to use custom unreal engine editor features
          if (Target.bBuildEditor)
          {
              PrivateDependencyModuleNames.AddRange(new string[] { "UnrealHelperEditor" });
          }
      }
  ```
  
  - to use specific engine version specify branch e.g. `-b UE5.4`

    ```bash
    git submodule add -b UE5.4 https://github.com/Ciberusps/unreal-helper-library.git ./Plugins/UnrealHelperLibrary
    ```

  > [!NOTE]
  > Don't forget to update your `README.md` with instructions on how to setup - `git submodule update --init --recursive` and how to update submodules/plugin(s) - `git submodule update --remote`

  > [!NOTE]
  > Add `Editor Preferences -> Force Compilation on Startup` in `Config/EditorPerProjectUserSettings.ini` your team don't want to recompile plugin manually 😉

</details>

#### From marketplace:

https://www.fab.com/listings/9f7d82e9-bc72-42ff-b302-b3d6562bd4c8

## ⬆️ Update

From source:

- `git submodule update --remote` to update plugin from source

## 🧩 Modules

## 📄 Documentation

**[🪲 DebugSystem](#-debugsystem)**

Debug system for your game, in mid-size commands you always use limited set of debugging tools

> - Settings
>   - [DebugSystemSettings](#debugsystemsettings)
> - Subsystem
>   - [UHLDebugSystemSubsystem](#uhldebugsystemsubsystem)
> - AsyncActions
>   - [WaitDebugCategoryChange](#aa_waitdebugcategorychange)
> - [DebugBlueprintLibrary](#debugblueprintlibrary)
>   - [IsUHLDebugCategoryEnabled](#isuhldebugcategoryenabled)


### 🪲 DebugSystem

Powerful debug system for gameplay based on GameplayTags. So any game needs debug system, in mid-size commands you always use limited set of debugging tools
more always than others, so **DebugSystem** is as tool for creating your debug system as fast as possible by providing `DebugCategories`(gameplaytags) that can be enabled/disabled

Use case:
I want to have debug for AbilitySystem, it should turn on/off, available in editor between sessions and.....

Components:

##### DebugSystemSettings

- add new categories, turn on/off default state, every debug category is a tag
- `DebugCategoryComponents` (DCC)

##### Check is category enabled/subscribe on debug category state change

- `WaitDebugCategoryChange` - blueprint node to easier check `isDebugCategoryEnabled` or not and wait for its changes
- `IsUHLDebugSystemEnabled` - blueprint node
- `DebugCategoriesList` - UI component for quick integration in your debug menu

**Features:**

- you can create DebugCategoryComponents that activate/deactivate console commands, event in blueprints like GAS abilities
- you can even compose DebugCategoryComponents e.g. you want Collisions + HitBoxes, so you can create combined DebugCategory and add "DCC_Collisions" and "DCC_HitBoxes"
- you can "Block" other DebugCategories by tag
- WaitDebugCategoryChange

**How to add DebugCategory:**
1) ...
2) ...

How to subscribe on debug category change in C++

```c++
    UAA_WaitDebugCategoryChange* WaitDebugCategoryChangeTask = UAA_WaitDebugCategoryChange::WaitDebugCategoryChange(
        Actor->GetWorld(),
        YourGameplayTags::TAG_DebugCategory_Combat // same as FGameplayTag("DebugCategory.Something")
    );
    WaitDebugCategoryChangeTask->OnChange.AddUniqueDynamic(this, &UCombatSubsystem::OnDebugCategoryChanged);
    // on activation "OnDebugCategoryChanged" will be fired
    WaitDebugCategoryChangeTask->Activate();
```

##### DebugSystemSubsystem

##### `UHLDebugSystemSubsystem`

**UHLDebugSystemSubsystem** - subsystem that provides interaction with debug categories

##### DebugBlueprintLibrary

###### `IsUHLDebugCategoryEnabled`

Check is debug category enabled or not

