#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"

#include "BTT_ApproachSOSlot.generated.h"

struct FApproachSOSlotMemory {};

UCLASS()
class ZOMBIE_API UBTT_ApproachSOSlot : public UBTTask_BlackboardBase {
  GENERATED_BODY()
public:
  UBTT_ApproachSOSlot();

  // I would love to use FBlackboardKeySelector but I cannot figure out how to set a filter for it
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FName SOClaimedSlotKey;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
};
