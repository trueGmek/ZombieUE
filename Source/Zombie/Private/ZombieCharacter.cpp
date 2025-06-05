#include "ZombieCharacter.h"

AZombieCharacter::AZombieCharacter() {
  PrimaryActorTick.bCanEverTick = true;
  HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AZombieCharacter::UpdateMovementSpeed(const float NewMaxWalkSpeed) {
  Super::GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}

void AZombieCharacter::BeginPlay() {
  Super::BeginPlay();
}

void AZombieCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

float AZombieCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, AActor* DamageCauser) {
  HealthComponent->ApplyDamage(DamageAmount);
  return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
