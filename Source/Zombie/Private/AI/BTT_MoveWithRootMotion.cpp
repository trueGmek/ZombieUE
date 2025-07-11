#include "AI/BTT_MoveWithRootMotion.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

  ZombieCharacter = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
  ensureMsgf(ZombieCharacter, TEXT("Owner is not of type AZombieCharacter"));

  ZombieAnimInstance = Cast<UZombieAnimInstance>(ZombieCharacter->GetMesh()->GetAnimInstance());
  ensureMsgf(ZombieAnimInstance, TEXT("The AnimInstance is not of type ZombieAnimInstance"));

  CurrentPath = ZombieCharacter->RootMotionNavigationComponent->FindPath(Destination, Blackboard);

  if (!CurrentPath || CurrentPath->PathPoints.Num() == 0) {
    UE_LOG(LogTemp, Warning, TEXT("[%hs]: Generated path is null or has no PathPoints inside"), __PRETTY_FUNCTION__)
    return EBTNodeResult::Failed;
  }

  PathPointIndex = 0;
  ZombieAnimInstance->bShouldWalk = this->MovementType == EMovementType::Walk;
  ZombieAnimInstance->bShouldRun = this->MovementType == EMovementType::Run;

  //TODO: SET IS USED TO FALSE AFTER FINISHING THIS TASK
  ZombieCharacter->RootMotionNavigationComponent->SetIsUsed(true);

  return EBTNodeResult::InProgress;
}

void UBTT_MoveWithRootMotion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  if (ZombieCharacter == nullptr || !CurrentPath) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    return;
  }

  const FVector CurrentLocation = ZombieCharacter->GetActorLocation();
  const FVector FinalDestination = CurrentPath->PathPoints.Last();
  const FVector ProjCurrentLocation = UKismetMathLibrary::ProjectVectorOnToPlane(CurrentLocation, FVector::UpVector);

  if (FVector::DistSquared(ProjCurrentLocation, FinalDestination) <
      FMath::Square(AcceptanceRange.GetValue(Blackboard))) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return;
  }

  const FVector TargetPoint = CurrentPath->PathPoints[PathPointIndex];

  if (FVector::DistSquared(ProjCurrentLocation, TargetPoint) < FMath::Square(NextPointRadius.GetValue(Blackboard))) {
    PathPointIndex++;
    if (PathPointIndex >= CurrentPath->PathPoints.Num()) {
      FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
      return;
    }
  }

  // Smoothly rotate the actor to face the current target point
  const FRotator CurrentRotation = ZombieCharacter->GetActorRotation();
  const FRotator TargetRotation =
      UKismetMathLibrary::FindLookAtRotation(CurrentLocation, CurrentPath->PathPoints[PathPointIndex]);
  const FRotator NewRotation =
      FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed.GetValue(Blackboard));

  // Only update Yaw to prevent the character from tilting up/down
  ZombieCharacter->SetActorRotation(FRotator(0.F, NewRotation.Yaw, 0.F));
}

void UBTT_MoveWithRootMotion::OnTaskFinished(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) {

  UE_LOG(LogTemp, Display, TEXT("Stoping movement. Disabling the animation flags"));
  ZombieAnimInstance->bShouldWalk = false;
  ZombieAnimInstance->bShouldRun = false;
  this->PathPointIndex = 0;
}
