#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "CoreMinimal.h"

#include "BTDecorator_IsInRange.generated.h"

/**
 * Checks whether the agent is in a certain distance to an actor.
 */
UCLASS()
class ZOMBIE_API UBTDecorator_IsInRange : public UBTDecorator {
  GENERATED_BODY()

  UBTDecorator_IsInRange(const FObjectInitializer& ObjectInitializer);
  virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
  virtual FString GetStaticDescription() const override;

protected:
  UPROPERTY(EditAnywhere, Category = Blackboard)
  FBlackboardKeySelector Actor;

  UPROPERTY(Category = Blackbaoard, EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
  FValueOrBBKey_Float Range;
};
