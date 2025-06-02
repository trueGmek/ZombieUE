// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"

#include "AIController.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "HAL/Platform.h"
#include "ZombieCharacter.h"

AZombieAIController::AZombieAIController() {
  AIperceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Sight"));
}

void AZombieAIController::BeginPlay() {
  GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString("BeginPlay"));
}

void AZombieAIController::OnPossess(APawn *InPawn) {
  Super::OnPossess(InPawn);

  auto *zombieCharacter = Cast<AZombieCharacter>(InPawn);

  if (zombieCharacter == nullptr)
    return;
  
  ensureMsgf(BehaviorTree, TEXT("Behaviour tree is not set"));
  bool bIsRunning = RunBehaviorTree(BehaviorTree);
  ensureMsgf(bIsRunning, TEXT("The behaviour tree is not running"));

  GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red,
                                   FString("RunBehaviorTree resulted with: ") +
                                       FString(bIsRunning ? "true" : "false"));
}
