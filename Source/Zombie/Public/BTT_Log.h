#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTT_Log.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIE_API UBTT_Log : public UBTTaskNode {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
  FString message;

public:
  UBTT_Log();

  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

  virtual FString GetStaticDescription() const override;
};
