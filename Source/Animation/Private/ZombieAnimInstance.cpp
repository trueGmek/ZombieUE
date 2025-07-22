#include "ZombieAnimInstance.h"

void UZombieAnimInstance::SetFlag(EZombieAnimKeys ZombieAnimKey, bool bValue) {
  switch (ZombieAnimKey) {
  case EZombieAnimKeys::GetHit:
    bIsHit = bValue;
    break;
  case EZombieAnimKeys::IsAttacking:
    bIsAttacking = bValue;
    break;
  case EZombieAnimKeys::IsDead:
    bIsDying = bValue;
    break;
  case EZombieAnimKeys::IsFeeding:
    bIsFeeding = bValue;
    break;
  case EZombieAnimKeys::IsScreaming:
    bIsScreaming = bValue;
    break;
  case EZombieAnimKeys::ShouldRun:
    bShouldRun = bValue;
    break;
  case EZombieAnimKeys::ShouldWalk:
    bShouldWalk = bValue;
    break;
  default:
    UE_LOG(LogTemp, Error, TEXT("You passed an unsupported EZombieAnimKey"));
    break;
  }
}
