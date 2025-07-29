#include "AI/BTT_PlayMontage.h"
#include "AI/ZombieAILogCattegory.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AssertionMacros.h"

UBTT_PlayMontage::UBTT_PlayMontage(const FObjectInitializer& ObjectInitializer) {
  NodeName = "Play Montage";

  bTickIntervals = false;
  bCreateNodeInstance = true;

  BlackboardKey.AddObjectFilter(
      this, GET_MEMBER_NAME_CHECKED(UBTT_PlayMontage, BlackboardKey), UAnimMontage::StaticClass());
}

EBTNodeResult::Type UBTT_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  UAnimMontage* montage = Cast<UAnimMontage>(
      OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));

  this->OwnerComp = &OwnerComp;

  AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();

  if (AnimInstance == nullptr || montage == nullptr) {
    FinishLatentAbort(OwnerComp);
  }

  AnimInstance->OnMontageEnded.AddDynamic(this, &UBTT_PlayMontage::OnMontageEnded);
  float time = AnimInstance->Montage_Play(montage);

  // Animation failed to play
  if (FMath::IsNearlyZero(time)) {
    FinishLatentAbort(OwnerComp);
  }

  return EBTNodeResult::InProgress;
}

void UBTT_PlayMontage::OnTaskFinished(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) {

  UE_LOG(ZombieAI, Display, TEXT("OnTaskFinished"));

  AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTT_PlayMontage::OnMontageEnded);
}

void UBTT_PlayMontage::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) {

  UE_LOG(ZombieAI, Display, TEXT("OnMontageEnded"));

  if (bInterrupted) {
    FinishLatentAbort(*OwnerComp);

    return;
  }

  FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
