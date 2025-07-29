#include "ZombieAIController.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BlackboardValuesSetter.h"
#include "HAL/Platform.h"
#include "TimerManager.h"
#include "ZombieCharacter.h"

AZombieAIController::AZombieAIController() {
  AIperceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Sight"));
  BlackboardValuesSetter = CreateDefaultSubobject<UBlackboardValuesSetter>(TEXT("BlackboardValuesSetter"));
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
    GetWorldTimerManager().SetTimer(
        SeenPlayerTimerHandle, this, &AZombieAIController::LoseEnemyReference, FollowAfterLosingSightPeriod, false);
  }
}

void AZombieAIController::LoseEnemyReference() const {
  BlackboardComponent->SetValueAsObject(EnemyBlackboardKey, nullptr);
}

void AZombieAIController::OnPossess(APawn* InPawn) {
  Super::OnPossess(InPawn);
  ZombieCharacter = Cast<AZombieCharacter>(InPawn);

  if (ZombieCharacter == nullptr || BehaviorTree == nullptr) {
    UE_LOG(LogTemp, Error, TEXT("Error when possessing an actor"));
    return;
  }

  SetUpBehaviorTree();
  ZombieCharacter->OnTakeDamage.AddDynamic(this, &AZombieAIController::SetDamageBlackboardFlag);
  ZombieCharacter->HealthComponent->OnDeath.AddDynamic(this, &AZombieAIController::HandleDeathLogic);
}

void AZombieAIController::SetUpBehaviorTree() {
  bool bIsRunning = RunBehaviorTree(BehaviorTree);
  ensureMsgf(bIsRunning, TEXT("The behaviour tree is not running"));

  UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(GetBrainComponent());
  BTComponent->SetDynamicSubtree(ChaseInjectionTag, ChaseBehaviorTree.Get());
}

void AZombieAIController::SetDamageBlackboardFlag() {
  BlackboardComponent->SetValueAsBool(IsHitBlackboardKey, true);
}

void AZombieAIController::HandleDeathLogic() {
  // After this stage the BT should consider the agent to be dead
  BlackboardComponent->SetValueAsBool(IsDeadBlackobardKey, true);

  GetWorldTimerManager().SetTimer(
      DestroyAgentTimerHandle, this, &AZombieAIController::DestroyAgent, DestroyAgentTimeDelay);
}

void AZombieAIController::DestroyAgent() {
  UnPossess();
  if (!(ZombieCharacter->Destroy() && Destroy())) {
    UE_LOG(LogTemp, Error, TEXT("Could not destroy agent!"));
  }
}
