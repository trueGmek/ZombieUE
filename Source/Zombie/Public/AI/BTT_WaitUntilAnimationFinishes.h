#pragma once

#include "AnimationProgressComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTT_WaitUntilAnimationFinishes.generated.h"

UCLASS()
class ZOMBIE_API UBTT_WaitUntilAnimationFinishes : public UBTTaskNode {
  GENERATED_BODY()

public:
  UBTT_WaitUntilAnimationFinishes();
  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
  bool HasAnimationFinished(UAnimationProgressComponent* AnimationProgressComponent);
};
