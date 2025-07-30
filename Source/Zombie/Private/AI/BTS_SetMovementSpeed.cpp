#include "AI/BTS_SetMovementSpeed.h"
#include "AIController.h"
#include "BehaviorTree/BTNode.h"
#include "GameFramework/Character.h"

UBTS_SetMovementSpeed::UBTS_SetMovementSpeed(const FObjectInitializer& ObjectInitializer) {
  NodeName = "Set Movement Speed";
  INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTS_SetMovementSpeed::InitializeMemory(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const {
  InitializeNodeMemory<FSetMovementSpeedMemory>(NodeMemory, InitType);
}

void UBTS_SetMovementSpeed::CleanupMemory(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const {
  CleanupNodeMemory<FSetMovementSpeedMemory>(NodeMemory, CleanupType);
}

uint16 UBTS_SetMovementSpeed::GetInstanceMemorySize() const {
  return sizeof(FSetMovementSpeedMemory);
}

void UBTS_SetMovementSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  UCharacterMovementComponent* movementComponent = OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement();
  ensure(movementComponent);

  FSetMovementSpeedMemory* thisNodeMemory = CastInstanceNodeMemory<FSetMovementSpeedMemory>(NodeMemory);
  ensure(thisNodeMemory);

  thisNodeMemory->MovementComponent = movementComponent;
  thisNodeMemory->PreviousMovementData =
      FMovementData{movementComponent->MaxWalkSpeed, movementComponent->MaxAcceleration};

  movementComponent->MaxWalkSpeed = MovementData.MaxMovementSpeed.GetValue(OwnerComp.GetBlackboardComponent());
  movementComponent->MaxAcceleration = MovementData.MaxAcceleration.GetValue(OwnerComp.GetBlackboardComponent());
}

void UBTS_SetMovementSpeed::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  const FSetMovementSpeedMemory* thisNodeMemory = CastInstanceNodeMemory<FSetMovementSpeedMemory>(NodeMemory);
  ensure(thisNodeMemory);

  thisNodeMemory->MovementComponent->MaxWalkSpeed =
      thisNodeMemory->PreviousMovementData.MaxMovementSpeed.GetValue(OwnerComp.GetBlackboardComponent());

  thisNodeMemory->MovementComponent->MaxAcceleration =
      thisNodeMemory->PreviousMovementData.MaxAcceleration.GetValue(OwnerComp.GetBlackboardComponent());
}
