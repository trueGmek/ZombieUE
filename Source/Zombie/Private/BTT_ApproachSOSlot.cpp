#include "BTT_ApproachSOSlot.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "Engine/World.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"

// TODO: CHECK IF THIS WILL WORK OR SHOULD I CHANGE THIS TO A DIFFERENT CONSTRTUCTOR
// UBTT_ApproachSOSlot::UBTT_ApproachSOSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
UBTT_ApproachSOSlot::UBTT_ApproachSOSlot() {
  bCreateNodeInstance = false;
  bNotifyTick = false;
  bNotifyTaskFinished = false;

  BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_ApproachSOSlot, BlackboardKey));
}

EBTNodeResult::Type UBTT_ApproachSOSlot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
  USmartObjectSubsystem* SOSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

  FSmartObjectClaimHandle ClaimHandle =
      BlackboardComponent->GetValue<UBlackboardKeyType_SOClaimHandle>(SOClaimedSlotKey);

  if (!ClaimHandle.IsValid()) {
    return EBTNodeResult::Failed;
  }
  FTransform SOTransform = SOSubsystem->GetSlotTransform(ClaimHandle).GetValue();
  BlackboardComponent->SetValueAsVector(BlackboardKey.SelectedKeyName, SOTransform.GetLocation());

  return EBTNodeResult::Succeeded;
}
