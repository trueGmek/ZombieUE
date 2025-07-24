#include "AI/BTS_SetAnimationFlag.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "ZombieCharacter.h"

UBTS_SetAnimationFlag::UBTS_SetAnimationFlag(const FObjectInitializer& ObjectInitializer) {
  NodeName = "Setting animation flag";
  bTickIntervals = false;
  INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTS_SetAnimationFlag::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  AZombieCharacter* zombieCharacter = OwnerComp.GetAIOwner()->GetPawn<AZombieCharacter>();
  UZombieAnimInstance* animInstance = Cast<UZombieAnimInstance>(zombieCharacter->GetMesh()->GetAnimInstance());
  animInstance->SetFlag(ZombieAnimKeys, bValue);
}
void UBTS_SetAnimationFlag::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  AZombieCharacter* zombieCharacter = OwnerComp.GetAIOwner()->GetPawn<AZombieCharacter>();
  UZombieAnimInstance* animInstance = Cast<UZombieAnimInstance>(zombieCharacter->GetMesh()->GetAnimInstance());
  animInstance->SetFlag(ZombieAnimKeys, !bValue);
}
