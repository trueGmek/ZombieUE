// Copyright Epic Games, Inc. All Rights Reserved.

#include "Zombie.h"
#include "GameplayDebugger.h"
#include "Modules/ModuleManager.h"
#include "Tools/ZombieAIDebugger.h"

void FZombie::StartupModule() {

#if WITH_GAMEPLAY_DEBUGGER
  IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();

  GameplayDebuggerModule.RegisterCategory(
      FZombieAIDebugger::CategoryName,
      IGameplayDebugger::FOnGetCategory::CreateStatic(&FZombieAIDebugger::MakeInstance),
      EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
      FZombieAIDebugger::CategoryNumber);
  GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FZombie::ShutdownModule() {

#if WITH_GAMEPLAY_DEBUGGER
  if (IGameplayDebugger::IsAvailable()) {
    IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
    GameplayDebuggerModule.UnregisterCategory(FZombieAIDebugger::CategoryName);
    GameplayDebuggerModule.NotifyCategoriesChanged();
  }
#endif
}

IMPLEMENT_PRIMARY_GAME_MODULE(FZombie, Zombie, "Zombie");
