#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTT_SetBBValue.generated.h"

/*
 * Class for setting a bool value in Blackboard
 * TODO: EXTEND THIS CLASS TO SET OTHER TYPES
 */
UCLASS()
class ZOMBIE_API UBTT_SetBBValue : public UBTTaskNode {

  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Blackboard")
  FBlackboardKeySelector Key;

  UPROPERTY(EditAnywhere, Category = "Blackboard")
  bool bValue;

  UBTT_SetBBValue();
  virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
  virtual FString GetStaticDescription() const override;
};
