#pragma once

#include "AnimationProgressComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "CoreMinimal.h"
#include "ZombieAnimInstance.h"
#include "ZombieCharacter.h"

#include "BTT_MoveWithRootMotion.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
  Walk UMETA(DisplayName = "Walk"),
  Run UMETA(DisplayName = "Run"),
};

UCLASS()
class ZOMBIE_API UBTT_MoveWithRootMotion : public UBTTaskNode {

  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, Category = Blackboard)
  EMovementType MovementType;

  UPROPERTY(EditAnywhere, Category = Blackboard)
  FBlackboardKeySelector Destination;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "0.1", UIMin = "0.1"))
  FValueOrBBKey_Float RotationSpeed;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
  FValueOrBBKey_Float AcceptanceRange;

  UPROPERTY()
  TObjectPtr<UZombieAnimInstance> ZombieAnimInstance;

  UPROPERTY()
  TObjectPtr<AZombieCharacter> ZombieCharacter;

private:
  TObjectPtr<UBlackboardComponent> Blackboard;

public:
  UBTT_MoveWithRootMotion();

  virtual FString GetStaticDescription() const override;
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
  virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
  FVector GetDestinationLocation(UBehaviorTreeComponent& OwnerComp);
};
