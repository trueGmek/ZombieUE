#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "AnimationProgressComponent.generated.h"

UCLASS(ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class ANIMATION_API UAnimationProgressComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UAnimationProgressComponent();

  UPROPERTY(BlueprintReadOnly)
  bool bInProgress{false};

  UPROPERTY(BlueprintReadOnly)
  float Progress;

  UPROPERTY(BlueprintReadOnly)
  float ElapsedTime{0};
};
