#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay() {
  Super::BeginPlay();
  CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float Amount) {
  CurrentHealth -= Amount;
  OnHealthChanged.Broadcast(CurrentHealth);

  if (CurrentHealth <= 0) {
    OnDeath.Broadcast();
  }
}

void UHealthComponent::ApplyHealing(float Amount) {
  CurrentHealth += Amount;
  OnHealthChanged.Broadcast(CurrentHealth);
}
