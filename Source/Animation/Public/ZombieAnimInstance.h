#pragma once

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.h"
#include "CoreMinimal.h"

#include "ZombieAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EZombieAnimKeys : uint8 {
  GetHit UMETA(DisplayName = "Get Hit"),
  ShouldWalk UMETA(DisplayName = "Should Walk"),
  ShouldRun UMETA(DisplayName = "Should Run"),
  IsAttacking UMETA(DisplayName = "Is Attacking"),
  IsScreaming UMETA(DisplayName = "Is Screaming"),
  IsFeeding UMETA(DisplayName = "Is Feeding"),
};

/*
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
