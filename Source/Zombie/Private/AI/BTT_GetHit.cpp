#include "AI/BTT_GetHit.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieCharacter.h"

UBTT_GetHit::UBTT_GetHit() {
  bNotifyTick = true;
  bNotifyTaskFinished = true;
  bCreateNodeInstance = false;
  IsHitKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_GetHit, IsHitKey));
}

EBTNodeResult::Type UBTT_GetHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  if (OwnerComp.GetAIOwner() == nullptr) {
    return EBTNodeResult::Failed;
  }

  return EBTNodeResult::InProgress;
}

void UBTT_GetHit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

  TObjectPtr<AZombieCharacter> zombieCharacter = OwnerComp.GetAIOwner()->GetPawn<AZombieCharacter>();
  if (HasAnimationFinished(zombieCharacter->AnimationProgressComponent.Get())) {
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsHitKey.SelectedKeyName, false);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

    return;
  }

  FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

bool UBTT_GetHit::HasAnimationFinished(UAnimationProgressComponent* AnimationProgressComponent) {
  float const CompletedTreshlod = 0.98F;
  return AnimationProgressComponent->Progress >= CompletedTreshlod;
}
