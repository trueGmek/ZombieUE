#include "ZombieCharacter.h"
#include "CharacterAnimInstance.h"
#include "Components/RootMotionNavigationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HAL/Platform.h"

AZombieCharacter::AZombieCharacter() {
  PrimaryActorTick.bCanEverTick = true;
  HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
  AnimationProgressComponent = CreateDefaultSubobject<UAnimationProgressComponent>(TEXT("AnimationProgressComponent"));
  RootMotionNavigationComponent = CreateDefaultSubobject<URootMotionNavigationComponent>(TEXT("RootMotionNavigationComponent"));
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

float AZombieCharacter::TakeDamage(float DamageAmount,
                                   struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator,
                                   AActor* DamageCauser) {

  HealthComponent->ApplyDamage(DamageAmount);

  TObjectPtr<UCharacterAnimInstance> characterAnmIstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

  characterAnmIstance->bIsHit = true;
  OnTakeDamage.Broadcast();

  return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
