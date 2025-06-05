// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class ZOMBIE_API AZombieCharacter : public ACharacter {
  GENERATED_BODY()

public:
  AZombieCharacter();

  UFUNCTION(BlueprintCallable)
  void UpdateMovementSpeed(float NewMaxWalkSpeed);

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;
  virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                           class AController* EventInstigator, AActor* DamageCauser) override;

private:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UHealthComponent> HealthComponent;
};
