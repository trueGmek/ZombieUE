#include "BTT_FindSmartObject.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "Engine/World.h"
#include "SmartObjectSubsystem.h"

UBTT_FindSmartObject::UBTT_FindSmartObject() {
  bCreateNodeInstance = false;
  bNotifyTick = false;
  bNotifyTaskFinished = false;
}

EBTNodeResult::Type UBTT_FindSmartObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  AActor* Actor = OwnerComp.GetAIOwner()->GetPawn();
  ensureMsgf(Actor, TEXT("Actor is a nullptr"));

  USmartObjectSubsystem* SmartObjectSubsystem = OwnerComp.GetWorld()->GetSubsystem<USmartObjectSubsystem>();
  ensureMsgf(SmartObjectSubsystem, TEXT("SmartObjectSubsystem is a nullptr"));

  FSmartObjectRequest Request{FBox, RequestFilter};
  FSmartObjectRequestResult Result = SmartObjectSubsystem->FindSmartObject(Request, Actor);

  if (!Result.IsValid()) {
    UE_LOG(LogTemp, Display, TEXT("FindSmartObjectRequestResult IS INVALID"));
    return EBTNodeResult::Failed;
  }

  bool bCanBeCalimed = SmartObjectSubsystem->CanBeClaimed(Result.SlotHandle);
  if (!bCanBeCalimed) {
    UE_LOG(LogTemp, Display, TEXT("SmartObjectSlotHandle CAN NOT BE CLAIMED"));
    return EBTNodeResult::Failed;
  }

  FSmartObjectClaimHandle ClaimHandle =
      SmartObjectSubsystem->MarkSlotAsClaimed(Result.SlotHandle, ESmartObjectClaimPriority::MAX);

  if (!ClaimHandle.IsValid()) {
    UE_LOG(LogTemp, Display, TEXT("SmartObjectClaimHandle IS INVALID"));
    return EBTNodeResult::Failed;
  }

  UBlackboardComponent* BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
  ensureMsgf(BB, TEXT("BB is a nullptr"));
  BB->SetValue<UBlackboardKeyType_SOClaimHandle>(SOClaimHandleKey, ClaimHandle);
  UE_LOG(LogTemp, Display, TEXT("Set the claim handle to he BlackboardComponent"));

  return EBTNodeResult::Succeeded;
}
