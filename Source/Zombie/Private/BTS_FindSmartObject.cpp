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
  FBox relativeBox = SearchBox.MoveTo(Actor->GetActorLocation());
  FSmartObjectRequest Request{relativeBox, RequestFilter};
  DrawDebugBox(GetWorld(), relativeBox.GetCenter(), relativeBox.GetExtent(), Actor->GetActorRotation().Quaternion(),
               FColor::Red);
  FSmartObjectClaimHandle ClaimHandle = TryClaimingSmartObject(Request, Actor);


  UBlackboardComponent* BB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
  if (ClaimHandle.IsValid()) {
    BB->SetValue<UBlackboardKeyType_SOClaimHandle>(BlackboardKey.GetSelectedKeyID(), ClaimHandle);
    UE_LOG(LogTemp, Display, TEXT("Set the claim handle to he BlackboardComponent"));
  }
}

FSmartObjectClaimHandle UBTS_FindSmartObject::TryClaimingSmartObject(FSmartObjectRequest& Request, AActor* Actor) {
  USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

  FSmartObjectRequestResult Result = SmartObjectSubsystem->FindSmartObject(Request, Actor);

  if (!Result.IsValid()) {
    return {};
  }

  UE_LOG(LogTemp, Display, TEXT("FindSmartObjectRequestResult IS VALID"));

  bool bCanBeCalimed = SmartObjectSubsystem->CanBeClaimed(Result.SlotHandle);
  if (!bCanBeCalimed) {
    return {};
  }

  UE_LOG(LogTemp, Display, TEXT("SmartObjectSlotHandle CAN BE CLAIMED"));

  FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem->MarkSlotAsClaimed(Result.SlotHandle, ClaimPriority);

  if (!ClaimHandle.IsValid()) {
    return {};
  }

  UE_LOG(LogTemp, Display, TEXT("SmartObjectClaimHandle IS VALID"));

  return ClaimHandle;
}
