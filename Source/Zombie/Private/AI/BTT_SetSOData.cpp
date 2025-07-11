#include "AI/BTT_SetSOData.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "Engine/World.h"
#include "Logging/LogVerbosity.h"
#include "SmartObjectComponent.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"

// TODO: CHECK IF THIS WILL WORK OR SHOULD I CHANGE THIS TO A DIFFERENT CONSTRTUCTOR
// UBTT_ApproachSOSlot::UBTT_ApproachSOSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
UBTT_SetSOData::UBTT_SetSOData() {
  bCreateNodeInstance = false;
  bNotifyTick = false;
  bNotifyTaskFinished = false;

  SOSlotLocationSelector.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_SetSOData, SOSlotLocationSelector));
  SOActorLocationSelecotr.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_SetSOData, SOActorLocationSelecotr));
}

void UBTT_SetSOData::InitializeFromAsset(UBehaviorTree& Asset) {
  Super::InitializeFromAsset(Asset);

  UBlackboardData* BBAsset = GetBlackboardAsset();
  if (BBAsset != nullptr) {
    SOSlotLocationSelector.ResolveSelectedKey(*BBAsset);
    SOActorLocationSelecotr.ResolveSelectedKey(*BBAsset);
    SOClaimedSlotSelector.ResolveSelectedKey(*BBAsset);
  } else {
    UE_LOG(LogBehaviorTree, Error,
           TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
  }
}

EBTNodeResult::Type UBTT_SetSOData::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();
  USmartObjectSubsystem* SOSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

  FSmartObjectClaimHandle ClaimHandle =
      BBComponent->GetValue<UBlackboardKeyType_SOClaimHandle>(SOClaimedSlotSelector.GetSelectedKeyID());

  if (!ClaimHandle.IsValid()) {
    return EBTNodeResult::Failed;
  }
  FTransform SOTransform = SOSubsystem->GetSlotTransform(ClaimHandle).GetValue();
  AActor* SOActor = SOSubsystem->GetSmartObjectComponent(ClaimHandle)->GetOwner();

  BBComponent->SetValue<UBlackboardKeyType_Vector>(SOSlotLocationSelector.GetSelectedKeyID(), SOTransform.GetLocation());
  BBComponent->SetValue<UBlackboardKeyType_Vector>(SOActorLocationSelecotr.GetSelectedKeyID(),
                                                   SOActor->GetActorLocation());

  return EBTNodeResult::Succeeded;
}
