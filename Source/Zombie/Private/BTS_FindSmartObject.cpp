#include "BTS_FindSmartObject.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/MathFwd.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"

UBTS_FindSmartObject::UBTS_FindSmartObject(const FObjectInitializer& ObjectInitializer) {
  UE_LOG(LogTemp, Display, TEXT("CREATING UBTS_FindSmartObject"));

  NodeName = "Search for Smart Objects";

  bTickIntervals = true;

  // TODO: add filter for smart object handle
  //  BlackboardKey.AddStringFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DefaultFocus, BlackboardKey));
}

void UBTS_FindSmartObject::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

  AActor* Actor = OwnerComp.GetAIOwner()->GetPawn();
  FVector Extent = SearchBox.GetExtent();
  FBox relativeBox = SearchBox.TransformBy(Actor->GetTransform());
  FSmartObjectRequest Request{relativeBox, RequestFilter};
  DrawDebugBox(GetWorld(), relativeBox.GetCenter(), relativeBox.GetExtent(), FQuat::Identity, FColor::Red);
  FSmartObjectClaimHandle ClaimHandle = TryClaimingSmartObject(Request, Actor);

  UBlackboardComponent* BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
  if (ClaimHandle.IsValid()) {
    BB->SetValue<UBlackboardKeyType_SOClaimHandle>(BlackboardKey.GetSelectedKeyID(), ClaimHandle);
    UE_LOG(LogTemp, Display, TEXT("Set the claim handle to he BlackboardComponent"));
  }
}

FSmartObjectClaimHandle UBTS_FindSmartObject::TryClaimingSmartObject(FSmartObjectRequest& Request, AActor* Actor) {
  USmartObjectSubsystem* SOSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

  TArray<FSmartObjectRequestResult> Results{};
  bool bSucceeded = SOSubsystem->FindSmartObjects(Request, Results, Actor);

  if (!bSucceeded) {
    return {};
  }

  UE_LOG(LogTemp, Display, TEXT("FindSmartObjectRequestResult IS VALID"));

  for (auto RequestResult : Results) {

    DrawDebugSphere(GetWorld(), SOSubsystem->GetSlotTransform(RequestResult)->GetLocation(), 10.0F, 10, FColor::Red,
                    true);
    bool bCanBeCalimed = SOSubsystem->CanBeClaimed(RequestResult.SlotHandle);
    if (!bCanBeCalimed) {
      continue;
    }

    UE_LOG(LogTemp, Display, TEXT("SmartObjectSlotHandle CAN BE CLAIMED"));

    FSmartObjectClaimHandle ClaimHandle = SOSubsystem->MarkSlotAsClaimed(RequestResult.SlotHandle, ClaimPriority);

    if (!ClaimHandle.IsValid()) {
      continue;
    }

    UE_LOG(LogTemp, Display, TEXT("SmartObjectClaimHandle IS VALID"));

    DrawDebugSphere(GetWorld(), SOSubsystem->GetSlotTransform(ClaimHandle)->GetLocation(), 10.0F, 10, FColor::Purple,
                    true);

    return ClaimHandle;
  }

  return {};
}
