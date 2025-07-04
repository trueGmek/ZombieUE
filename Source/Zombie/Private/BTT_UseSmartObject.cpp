#include "BTT_UseSmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "SmartObjectSubsystem.h"

UBTT_UseSmartObject::UBTT_UseSmartObject() {
  // TODO: USE NODE MEMMORY INSTEAD OF CREATING AN INSTANCE
  bCreateNodeInstance = true;
  bNotifyTick = true;
  bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_UseSmartObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  BBComponent = OwnerComp.GetBlackboardComponent();
  ensureMsgf(BBComponent, TEXT("BBComponent is a nullptr"));

  FSmartObjectClaimHandle ClaimHandle = BBComponent->GetValue<UBlackboardKeyType_SOClaimHandle>(SOClaimHandleKey);

  return EBTNodeResult::InProgress;
}

void UBTT_UseSmartObject::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {}
