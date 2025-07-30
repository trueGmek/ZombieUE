#pragma once

#include "BehaviorTree/BTService.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BTS_SetMovementSpeed.generated.h"

USTRUCT(BlueprintType)
struct FMovementData {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FValueOrBBKey_Float MaxMovementSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FValueOrBBKey_Float MaxAcceleration;
};

struct FSetMovementSpeedMemory {
  UCharacterMovementComponent* MovementComponent;
  FMovementData PreviousMovementData;
};

UCLASS()
class ZOMBIE_API UBTS_SetMovementSpeed : public UBTService {
  GENERATED_BODY()
public:
  UBTS_SetMovementSpeed(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FMovementData MovementData;

protected:
  ZOMBIE_API virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
  ZOMBIE_API virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
  ZOMBIE_API virtual void
  InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
  ZOMBIE_API virtual void
  CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
  ZOMBIE_API virtual uint16 GetInstanceMemorySize() const override;

private:
};
