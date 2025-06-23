#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"

#include "CharacterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ECharacterAnimKeys : uint8 {
  GetHit UMETA(DisplayName = "Get Hit"),
};

/**
 * Base class for any animation instances of characters in the game
 */
UCLASS()
class ANIMATION_API UCharacterAnimInstance : public UAnimInstance {
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bIsHit;
};
