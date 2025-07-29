#pragma once

#include "Animation/AnimInstance.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "BTT_PlayMontage.generated.h"

UCLASS()
class ZOMBIE_API UBTT_PlayMontage : public UBTTask_BlackboardBase {
  GENERATED_BODY()
public:
  UBTT_PlayMontage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
  UPROPERTY()
  UBehaviorTreeComponent* OwnerComp;
  UPROPERTY()
  UAnimInstance* AnimInstance;

  UFUNCTION()
  void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

  virtual void
  OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
