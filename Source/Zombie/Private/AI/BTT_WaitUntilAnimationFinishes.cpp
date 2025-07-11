#include "AI/BTT_WaitUntilAnimationFinishes.h"
#include "AIController.h"
#include "ZombieCharacter.h"

UBTT_WaitUntilAnimationFinishes::UBTT_WaitUntilAnimationFinishes() {
  bNotifyTick = true;
  bNotifyTaskFinished = true;
  bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTT_WaitUntilAnimationFinishes::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  if (OwnerComp.GetAIOwner() == nullptr) {
    return EBTNodeResult::Failed;
  }

  return EBTNodeResult::InProgress;
}

void UBTT_WaitUntilAnimationFinishes::TickTask(UBehaviorTreeComponent& OwnerComp,
                                               uint8* NodeMemory,
                                               float DeltaSeconds) {
  TObjectPtr<AZombieCharacter> zombieCharacter = OwnerComp.GetAIOwner()->GetPawn<AZombieCharacter>();
  if (HasAnimationFinished(zombieCharacter->AnimationProgressComponent.Get())) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return;
  }

  FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

bool UBTT_WaitUntilAnimationFinishes::HasAnimationFinished(UAnimationProgressComponent* AnimationProgressComponent) {
  float const CompletedTreshlod = 0.98F;
  return AnimationProgressComponent->Progress >= CompletedTreshlod;
}
