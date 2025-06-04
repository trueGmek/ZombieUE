// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "HAL/Platform.h"
#include "TimerManager.h"
#include "ZombieCharacter.h"

AZombieAIController::AZombieAIController() {
  AIperceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Sight"));
  AIperceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAIController::UpdatePerception);
}

void AZombieAIController::BeginPlay() {
  Super::BeginPlay();
  BlackboardComponent = GetBlackboardComponent();
}

void AZombieAIController::UpdatePerception(AActor* Actor, FAIStimulus Stimulus) {
  if (Actor != nullptr && Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed()) {

    GetWorldTimerManager().ClearTimer(SeenPlayerTimerHandle);
    BlackboardComponent->SetValueAsObject(EnemyBlackboardKey, Actor);
    BlackboardComponent->SetValueAsBool(LOSBlackboardKey, true);

  } else {
    BlackboardComponent->SetValueAsBool(LOSBlackboardKey, false);
    GetWorldTimerManager().SetTimer(SeenPlayerTimerHandle, this, &AZombieAIController::LoseEnemyReference,
                                    FollowAfterLosingSightPeriod, false);
  }
}

void AZombieAIController::LoseEnemyReference() const {
  BlackboardComponent->SetValueAsObject(EnemyBlackboardKey, nullptr);
}

void AZombieAIController::OnPossess(APawn* InPawn) {
  Super::OnPossess(InPawn);

  auto* zombieCharacter = Cast<AZombieCharacter>(InPawn);

  if (zombieCharacter == nullptr) {
    return;
  }

  ensureMsgf(BehaviorTree, TEXT("Behaviour tree is not set"));
  bool bIsRunning = RunBehaviorTree(BehaviorTree);
  ensureMsgf(bIsRunning, TEXT("The behaviour tree is not running"));
}
