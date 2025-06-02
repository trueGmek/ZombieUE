// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class ZOMBIE_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieCharacter();

	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeed(float NewMaxWalkSpeed);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UCharacterMovementComponent* CharacterMovement{nullptr};
};
