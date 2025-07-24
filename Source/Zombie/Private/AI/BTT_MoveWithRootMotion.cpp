#include "AI/BTT_MoveWithRootMotion.h"
#include "AIController.h"
#include "Animation/AnimData/AnimDataNotifications.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogVerbosity.h"
#include "Math/MathFwd.h"
#include "NavigationSystem.h"

UBTT_MoveWithRootMotion::UBTT_MoveWithRootMotion() {
  bNotifyTick = true;
  bNotifyTaskFinished = true;
  bCreateNodeInstance = true;

  Destination.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveWithRootMotion, Destination));
  Destination.AddObjectFilter(
      this, GET_MEMBER_NAME_CHECKED(UBTT_MoveWithRootMotion, Destination), AActor::StaticClass());
}

void UBTT_MoveWithRootMotion::InitializeFromAsset(UBehaviorTree& Asset) {
  Super::InitializeFromAsset(Asset);

  if (Asset.BlackboardAsset) {
    Destination.ResolveSelectedKey(*Asset.BlackboardAsset);
  }
}

FString UBTT_MoveWithRootMotion::GetStaticDescription() const {
  FString KeyDesc("invalid");

  if (Destination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
      Destination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
    KeyDesc = Destination.SelectedKeyName.ToString();
  }

  return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}

EBTNodeResult::Type UBTT_MoveWithRootMotion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  if (OwnerComp.GetOwner() == nullptr) {
    return EBTNodeResult::Failed;
  }

  Blackboard = OwnerComp.GetBlackboardComponent();
  ensureMsgf(Blackboard, TEXT("Blackboard is null"));

  ZombieCharacter = Cast<AZombieRootMotionCharacter>(OwnerComp.GetAIOwner()->GetPawn());
  ensureMsgf(ZombieCharacter, TEXT("Owner is not of type AZombieCharacter"));

  CurrentPath =
      ZombieCharacter->RootMotionNavigationComponent->FindPathToBlackboardDestination(Destination, Blackboard);

  if (!CurrentPath || CurrentPath->PathPoints.Num() == 0) {
    return EBTNodeResult::Failed;
  }

  PathPointIndex = 0;
  ZombieCharacter->RootMotionNavigationComponent->StartMovement(MovementType);

  return EBTNodeResult::InProgress;
}

void UBTT_MoveWithRootMotion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  if (ZombieCharacter == nullptr || !CurrentPath) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    return;
  }

  const FVector FinalDestination = CurrentPath->PathPoints.Last();

  FVector ProjCurrentLocation;
  bool bWasPointProjected = ZombieCharacter->RootMotionNavigationComponent->ProjectPointToNavMesh(
      ZombieCharacter->GetActorLocation(), ProjCurrentLocation);

  if (!bWasPointProjected) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    UE_LOG(LogTemp, Error, TEXT("Couldn't project current position to Navigation Mesh"));
    return;
  }

  if (FVector::DistSquared(ProjCurrentLocation, FinalDestination) <
      FMath::Square(AcceptanceRange.GetValue(Blackboard))) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return;
  }

  const FVector TargetPoint = CurrentPath->PathPoints[PathPointIndex];

  ManageRotation(ProjCurrentLocation, TargetPoint, DeltaSeconds);

  if (FVector::DistSquared(ProjCurrentLocation, TargetPoint) < FMath::Square(NextPointRadius.GetValue(Blackboard))) {
    PathPointIndex = FMath::Clamp(++PathPointIndex, 0, CurrentPath->PathPoints.Num() - 1);
  }
}

void UBTT_MoveWithRootMotion::ManageRotation(
    const FVector CurrentLocation, const FVector NextPointLocation, const float DeltaSeconds) {

  const FRotator CurrentRotation = ZombieCharacter->GetActorRotation();
  const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, NextPointLocation);

  const FRotator NewRotation =
      FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed.GetValue(Blackboard));

  ZombieCharacter->SetActorRotation(FRotator(0.F, NewRotation.Yaw, 0.F));
}

void UBTT_MoveWithRootMotion::OnTaskFinished(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) {

  UE_LOG(LogTemp, Display, TEXT("Stoping movement. Disabling the animation flags"));

  ZombieCharacter->RootMotionNavigationComponent->StopMovement();
  this->PathPointIndex = 0;
}
