#pragma once

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.h"
#include "CoreMinimal.h"

#include "ZombieAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class ANIMATION_API UZombieAnimInstance : public UCharacterAnimInstance {
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bShouldWalk;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bShouldRun;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bIsAttacking;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bIsDying;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bIsScreaming;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bIsFeeding;
};
