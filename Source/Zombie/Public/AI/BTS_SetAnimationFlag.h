#pragma once

#include "BehaviorTree/BTService.h"
#include "ZombieAnimInstance.h"

#include "BTS_SetAnimationFlag.generated.h"

UCLASS()
class ZOMBIE_API UBTS_SetAnimationFlag : public UBTService {
  GENERATED_BODY()
public:
  UBTS_SetAnimationFlag(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EZombieAnimKeys ZombieAnimKeys;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bValue;

protected:
  ZOMBIE_API virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
  ZOMBIE_API virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
