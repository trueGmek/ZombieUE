#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimationProgressComponent.h"
#include "CoreMinimal.h"

#include "AnimNotifyState_Progress.generated.h"

/**
 *
 */
UCLASS()
class ANIMATION_API UAnimNotifyState_Progress : public UAnimNotifyState {
  GENERATED_BODY()

private:
  ANIMATION_API void NotifyBegin(USkeletalMeshComponent* MeshComp,
                                 UAnimSequenceBase* Animation,
                                 float TotalDuration,
                                 const FAnimNotifyEventReference& EventReference) override;

  ANIMATION_API void NotifyTick(USkeletalMeshComponent* MeshComp,
                                UAnimSequenceBase* Animation,
                                float FrameDeltaTime,
                                const FAnimNotifyEventReference& EventReference) override;

  ANIMATION_API void NotifyEnd(USkeletalMeshComponent* MeshComp,
                               UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference) override;

public:
  UAnimNotifyState_Progress();

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
  TObjectPtr<UAnimationProgressComponent> AnimationProgressComponent;

private:
  float elapsedTime{0};
};
