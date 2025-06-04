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
  OnHealthChangedDelegate.Execute(CurrentHealth);

  if (CurrentHealth <= 0) {
    OnDeathDelegate.Execute();
  }
}

void UHealthComponent::ApplyHealing(float Amount) {
  CurrentHealth += Amount;
  OnHealthChangedDelegate.Execute(CurrentHealth);
}

void UHealthComponent::DebugDelegate(float CurrentHealth) {
  GLog->Log("");
}
