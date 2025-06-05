#pragma once

#include "Combat/DamageData.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, CurrentHealth);

UCLASS(ClassGroup = (Health), meta = (BlueprintSpawnableComponent))
class COMBAT_API UHealthComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UHealthComponent();

  void ApplyDamage(float Amount);
  void ApplyHealing(float Amount);

protected:
  virtual void BeginPlay() override;

public:
  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
  float CurrentHealth;

  UPROPERTY(EditAnywhere, Category = "Health")
  float MaxHealth{300};

  FOnHealthChangedDelegate OnHealthChanged;
  FOnDeathDelegate OnDeath;
};
