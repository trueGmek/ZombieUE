#include "AI/BTS_FindSmartObject.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/MathFwd.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"

UBTS_FindSmartObject::UBTS_FindSmartObject(const FObjectInitializer& ObjectInitializer) {
  UE_LOG(LogTemp, Display, TEXT("CREATING UBTS_FindSmartObject"));

  NodeName = "Search for Smart Objects";

  bTickIntervals = true;

  // TODO: ADD FILTER FOR SMART OBJECT HANDLE
}

void UBTS_FindSmartObject::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

  AActor* Actor = OwnerComp.GetAIOwner()->GetPawn();
  FVector Extent = SearchBox.GetExtent();
  FBox relativeBox = SearchBox.TransformBy(Actor->GetTransform());
  FSmartObjectRequest Request{relativeBox, RequestFilter};
  FSmartObjectClaimHandle ClaimHandle = TryClaimingSmartObject(Request, Actor);

  UBlackboardComponent* BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
  if (ClaimHandle.IsValid()) {
    BB->SetValue<UBlackboardKeyType_SOClaimHandle>(BlackboardKey.GetSelectedKeyID(), ClaimHandle);
  }
}

FSmartObjectClaimHandle UBTS_FindSmartObject::TryClaimingSmartObject(FSmartObjectRequest& Request, AActor* Actor) {
  USmartObjectSubsystem* SOSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

  TArray<FSmartObjectRequestResult> Results{};
  bool bSucceeded = SOSubsystem->FindSmartObjects(Request, Results, Actor);

  if (!bSucceeded) {
    return {};
  }

  for (auto RequestResult : Results) {

    bool bCanBeCalimed = SOSubsystem->CanBeClaimed(RequestResult.SlotHandle);
    if (!bCanBeCalimed) {
      continue;
    }

    FSmartObjectClaimHandle ClaimHandle = SOSubsystem->MarkSlotAsClaimed(RequestResult.SlotHandle, ClaimPriority);

    if (!ClaimHandle.IsValid()) {
      continue;
    }

    return ClaimHandle;
  }

  return {};
}
