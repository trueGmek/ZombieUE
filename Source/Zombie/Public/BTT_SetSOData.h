#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"

#include "BTT_SetSOData.generated.h"

struct FApproachSOSlotMemory {};

UCLASS()
class ZOMBIE_API UBTT_SetSOData : public UBTTaskNode {
  GENERATED_BODY()
public:
  UBTT_SetSOData();

  UPROPERTY(EditAnywhere, Category = Blackboard)
  struct FBlackboardKeySelector SOSlotLocationSelector;

  UPROPERTY(EditAnywhere, Category = Blackboard)
  struct FBlackboardKeySelector SOClaimedSlotSelector;

  UPROPERTY(EditAnywhere, Category = Blackboard)
  struct FBlackboardKeySelector SOActorLocationSelecotr;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
};
