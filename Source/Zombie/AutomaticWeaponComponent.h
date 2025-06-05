#pragma once

#include "AutomaticWeaponComponent.generated.h"

class UInputMappingContext;
class APlayerCharacter;
class UInputAction;

UENUM()
enum class EType { Projectile, Hitscan };

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIE_API UAutomaticWeaponComponent : public USkeletalMeshComponent {
  GENERATED_BODY()

public:
  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = Projectile)
  TSubclassOf<class AZombieProjectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  USoundBase* FireSound;

  /** AnimMontage to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  UAnimMontage* FireAnimation;

  /** Gun muzzle's offset from the characters location */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  FVector MuzzleOffset;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputMappingContext* FireMappingContext;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  UInputAction* FireAction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  float Cooldown{1};

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
  EType Type{EType::Projectile};

  // UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Gameplay)
  float Range{5000};

private:
  APlayerCharacter* Character;

  bool bShouldFire{false};
  float LastFireTime{0};

public:
  void BindInputs();
  UFUNCTION(BlueprintCallable, Category = "Weapons")
  bool AttachWeaponToPlayer(APlayerCharacter* TargetCharacter);

  virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction) override;

  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
  void Fire() const;
  void CacheFirePressed();
  void CacheFireReleased();
  void SpawnProjectile(AController* CharacterController, UWorld* World) const;
  void HitScan(AController* const CharacterController) const;
  void ManageFX() const;
};
