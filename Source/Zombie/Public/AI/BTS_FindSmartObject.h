#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SmartObjectSubsystem.h"

#include "BTS_FindSmartObject.generated.h"

UCLASS()
class ZOMBIE_API UBTS_FindSmartObject : public UBTService_BlackboardBase

{
  GENERATED_BODY()
public:
  UBTS_FindSmartObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FBox SearchBox;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FSmartObjectRequestFilter RequestFilter;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  ESmartObjectClaimPriority ClaimPriority;

protected:
  virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
  FSmartObjectClaimHandle TryClaimingSmartObject(FSmartObjectRequest& Request, AActor* Actor);
};
