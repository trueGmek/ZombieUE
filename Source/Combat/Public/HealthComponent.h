#pragma once

#include "Combat/DamageData.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(OnHealthChanged, float);
DECLARE_DELEGATE(OnDeath);

UCLASS(ClassGroup = (Health), meta = (BlueprintSpawnableComponent))
class COMBAT_API UHealthComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UHealthComponent();

  void ApplyDamage(float Amount);
  void ApplyHealing(float Amount);

protected:
  virtual void BeginPlay() override;
  virtual void DebugDelegate(float CurrentHealth);

public:
  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
  float CurrentHealth;

  UPROPERTY(EditAnywhere, Category = "Health")
  float MaxHealth{300};

  OnHealthChanged OnHealthChangedDelegate;
  OnDeath OnDeathDelegate;
};
