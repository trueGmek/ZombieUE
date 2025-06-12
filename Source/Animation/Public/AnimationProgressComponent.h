#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "AnimationProgressComponent.generated.h"

UCLASS(ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class ANIMATION_API UAnimationProgressComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UAnimationProgressComponent();

  bool bInProgress{false};
  float Progress;
  float ElapsedTime{0};
};
