#include "Components/RootMotionNavigationComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

URootMotionNavigationComponent::URootMotionNavigationComponent() {}

void URootMotionNavigationComponent::BeginPlay() {
  ZombieAnimInstance = Cast<UZombieAnimInstance>(GetOwner<ACharacter>()->GetMesh()->GetAnimInstance());
  NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

  ensureMsgf(ZombieAnimInstance, TEXT("ZombieAnimInstance reference is missing"));
  ensureMsgf(NavigationSystem, TEXT("NavigationSystem is null"));
}

UNavigationPath* URootMotionNavigationComponent::FindPathToBlackboardDestination(
    const FBlackboardKeySelector& Destination, const UBlackboardComponent* BlackboardComponent) {

  if (Destination.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
    UObject* KeyValue = BlackboardComponent->GetValue<UBlackboardKeyType_Object>(Destination.GetSelectedKeyID());
    AActor* TargetActor = Cast<AActor>(KeyValue);

    if (TargetActor != nullptr) {
      return FindPathToActor(TargetActor);
    }
  }
  if (Destination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
    FVector destination = BlackboardComponent->GetValue<UBlackboardKeyType_Vector>(Destination.GetSelectedKeyID());
    return FindPathToDestination(destination);
  }

  return nullptr;
}

UNavigationPath* URootMotionNavigationComponent::FindPathToDestination(const FVector& Destination) {
  return FindPathFromStartToDestination(GetOwner()->GetActorLocation(), Destination);
}

UNavigationPath* URootMotionNavigationComponent::FindPathToActor(AActor* Target) {
  return FindPathFromStartToActor(GetOwner()->GetActorLocation(), Target);
}

UNavigationPath* URootMotionNavigationComponent::FindPathFromStartToDestination(
    const FVector& StartPosition, const FVector& Destination) {
  auto* path =
      UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetOwner()->GetActorLocation(), Destination);

  CachedPath = path;

  return path;
}

UNavigationPath*
URootMotionNavigationComponent::FindPathFromStartToActor(const FVector& StartPosition, AActor* Target) {
  auto* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetOwner()->GetActorLocation(), Target);

  CachedPath = path;

  return path;
}

bool URootMotionNavigationComponent::ProjectPointToNavMesh(FVector const& InPosition, FVector& OutPosition) {

  FNavLocation projectedLocation{};
  bool bResult = NavigationSystem->ProjectPointToNavigation(InPosition, projectedLocation, QueryExtent);

  OutPosition = projectedLocation.Location;

  return bResult;
}

void URootMotionNavigationComponent::StartMovement(EMovementType MovementType) {

  ZombieAnimInstance->bShouldWalk = MovementType == EMovementType::Walk;
  ZombieAnimInstance->bShouldRun = MovementType == EMovementType::Run;
  bIsUsed = true;
}

void URootMotionNavigationComponent::StopMovement() {
  ZombieAnimInstance->bShouldWalk = false;
  ZombieAnimInstance->bShouldRun = false;
  bIsUsed = false;
}
