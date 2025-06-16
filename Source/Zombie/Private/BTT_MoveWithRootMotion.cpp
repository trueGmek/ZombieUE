#include "BTT_MoveWithRootMotion.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UBTT_MoveWithRootMotion::UBTT_MoveWithRootMotion() {
  bNotifyTick = true;
  bNotifyTaskFinished = true;
  bCreateNodeInstance = true;

  Destination.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveWithRootMotion, Destination));
  Destination.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveWithRootMotion, Destination),
                              AActor::StaticClass());
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

  ZombieAnimInstance->bShouldWalk = this->MovementType == EMovementType::Walk;
  ZombieAnimInstance->bShouldRun = this->MovementType == EMovementType::Run;

  return EBTNodeResult::InProgress;
}

void UBTT_MoveWithRootMotion::InitializeFromAsset(UBehaviorTree& Asset) {
  Super::InitializeFromAsset(Asset);

  if (Asset.BlackboardAsset) {
    Destination.ResolveSelectedKey(*Asset.BlackboardAsset);
  }
}

void UBTT_MoveWithRootMotion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  ensureMsgf(Destination.IsSet(), TEXT("Destination.IsSet was false!"));
  AActor* agent = OwnerComp.GetOwner();
  const FVector destinationLocation = GetDestinationLocation(OwnerComp);

  const FVector currentLocation = agent->GetActorLocation();

  FVector direction = (destinationLocation - currentLocation).GetSafeNormal();
  FQuat DesiredRot = direction.Rotation().Quaternion();

  // 2. Smoothly interpolate from current to desired
  FQuat CurrentRot = agent->GetActorQuat();
  FQuat NewRot = FMath::QInterpTo(CurrentRot, DesiredRot, DeltaSeconds, RotationSpeed.GetValue(Blackboard));
  agent->SetActorRotation(NewRot);
  if ((destinationLocation - currentLocation).Length() <= AcceptanceRange.GetValue(Blackboard)) {
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
  }
}

FVector UBTT_MoveWithRootMotion::GetDestinationLocation(UBehaviorTreeComponent& OwnerComp) {

  if (Destination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
    UObject* KeyValue = Blackboard->GetValue<UBlackboardKeyType_Object>(Destination.GetSelectedKeyID());
    AActor* TargetActor = Cast<AActor>(KeyValue);

    if (TargetActor != nullptr) {
      return TargetActor->GetActorLocation();
    }
    UE_LOG(LogTemp, Error,
           TEXT("Destination key was set to an object that is null or not an Actor! Please make sure  that the value "
                "is valid"));
  }
  if (Destination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {

    return Blackboard->GetValue<UBlackboardKeyType_Vector>(Destination.GetSelectedKeyID());
  }

  UE_LOG(LogTemp, Error, TEXT("Something went wrong! Quiting the task!"));
  FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
  return FVector::ZeroVector;
}
