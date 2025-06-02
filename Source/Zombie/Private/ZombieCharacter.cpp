#include "ZombieCharacter.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZombieCharacter::UpdateMovementSpeed(const float NewMaxWalkSpeed)
{
	CharacterMovement->MaxWalkSpeed = NewMaxWalkSpeed;
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovement = GetCharacterMovement();
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
