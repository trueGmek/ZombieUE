// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimationProgressComponent.h"
#include "Components/RootMotionNavigationComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "ZombieCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamageDelegate);

UCLASS()
class ZOMBIE_API AZombieCharacter : public ACharacter {
  GENERATED_BODY()

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UHealthComponent> HealthComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UAnimationProgressComponent> AnimationProgressComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URootMotionNavigationComponent> RootMotionNavigationComponent;

  FOnTakeDamageDelegate OnTakeDamage;

  AZombieCharacter();

  virtual void Tick(float DeltaTime) override;
  virtual float TakeDamage(
      float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
      AActor* DamageCauser) override;

  UFUNCTION(BlueprintCallable)
  void UpdateMovementSpeed(float NewMaxWalkSpeed);

protected:
  virtual void BeginPlay() override;
};
