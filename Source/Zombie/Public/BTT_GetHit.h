#pragma once

#include "AnimationProgressComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTT_GetHit.generated.h"

UCLASS()
class ZOMBIE_API UBTT_GetHit : public UBTTaskNode {

  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Blackboard")
  FBlackboardKeySelector IsHitKey;

private:
  bool HasAnimationFinished(UAnimationProgressComponent* AnimationProgressComponent);

public:
  UBTT_GetHit();
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
