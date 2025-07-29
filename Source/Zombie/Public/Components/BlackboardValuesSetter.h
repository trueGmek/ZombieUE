#pragma once

#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"

#include "BlackboardValuesSetter.generated.h"

UCLASS(ClassGroup = (Blackboard), meta = (BlueprintSpawnableComponent))
class ZOMBIE_API UBlackboardValuesSetter : public UActorComponent {
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere, Category = "General")
  TObjectPtr<UBlackboardData> BBAsset;

  UPROPERTY(EditAnywhere, Category = "GetHitValues")
  FBlackboardKeySelector GetHitAnimationKey;

  UPROPERTY(EditAnywhere, Category = "GetHitValues")
  TObjectPtr<UAnimMontage> GetHitAnimMontage;

protected:
  virtual void PostInitProperties() override;
  virtual void PostLoad() override;
  void SetValues(UBlackboardComponent& BlackboardComponent);
  virtual void BeginPlay() override;

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

private:
  void TryResolvingKey(FBlackboardKeySelector& key) const;
  void SetBlackboardKeySelectorFilters();
};
