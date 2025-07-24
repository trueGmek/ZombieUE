#pragma once

#include "AnimationProgressComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "CoreMinimal.h"
#include "NavigationPath.h"
#include "ZombieAnimInstance.h"
#include "ZombieRootMotionCharacter.h"

#include "BTT_MoveWithRootMotion.generated.h"

/**
 * Custom BT Task to move an actor using Root Motion.
 * It requests a path, follows it, smoothly rotates the actor,
 * and sets animation flags to drive the movement.
 */
UCLASS()
class ZOMBIE_API UBTT_MoveWithRootMotion : public UBTTaskNode {

  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = Blackboard)
  EMovementType MovementType;

  UPROPERTY(EditAnywhere, Category = Blackboard)
  FBlackboardKeySelector Destination;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "0.1", UIMin = "0.1"))
  FValueOrBBKey_Float RotationSpeed;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
  FValueOrBBKey_Float AcceptanceRange;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
  FValueOrBBKey_Float NextPointRadius{50.0F};

  UPROPERTY()
  TObjectPtr<AZombieRootMotionCharacter> ZombieCharacter;

  UPROPERTY()
  TObjectPtr<UNavigationPath> CurrentPath;

  /** Index of the current point on the path the actor is moving towards */
  int32 PathPointIndex{0};

private:
  TObjectPtr<UBlackboardComponent> Blackboard;

public:
  UBTT_MoveWithRootMotion();

  virtual FString GetStaticDescription() const override;
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
  virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
  virtual void
  OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
  void ManageRotation(FVector CurrentLocation, FVector NextPointLocation, float DeltaSeconds);
};
