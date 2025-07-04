#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTT_UseSmartObject.generated.h"

UCLASS()
class ZOMBIE_API UBTT_UseSmartObject : public UBTTaskNode {
  GENERATED_BODY()
public:
  UBTT_UseSmartObject();

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FName SOClaimHandleKey;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
  UPROPERTY()
  UBlackboardComponent* BBComponent;
};
