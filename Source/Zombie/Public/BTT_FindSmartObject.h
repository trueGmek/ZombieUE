#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "SmartObjectSubsystem.h"

#include "BTT_FindSmartObject.generated.h"

UCLASS()
class ZOMBIE_API UBTT_FindSmartObject : public UBTTaskNode {
  GENERATED_BODY()
public:
  UBTT_FindSmartObject();

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FBox FBox;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FSmartObjectRequestFilter RequestFilter;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FName SOClaimHandleKey;
 
protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
