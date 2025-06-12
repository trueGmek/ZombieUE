#include "AnimNotifyState_Progress.h"
#include "Animation/AnimSequenceBase.h"
#include "AnimationProgressComponent.h"
#include "Components/SkeletalMeshComponent.h"

UAnimNotifyState_Progress::UAnimNotifyState_Progress() {}

void UAnimNotifyState_Progress::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                            UAnimSequenceBase* Animation,
                                            float TotalDuration,
                                            const FAnimNotifyEventReference& EventReference) {

  if (MeshComp->GetOwner() == nullptr) {
    return;
  }

  AnimationProgressComponent = MeshComp->GetOwner()->GetComponentByClass<UAnimationProgressComponent>();
  elapsedTime = 0.0F;

  if (AnimationProgressComponent == nullptr) {
    return;
  }

  AnimationProgressComponent->Progress = 0.0F;
  AnimationProgressComponent->bInProgress = true;
  AnimationProgressComponent->ElapsedTime = elapsedTime;
};

void UAnimNotifyState_Progress::NotifyTick(USkeletalMeshComponent* MeshComp,
                                           UAnimSequenceBase* Animation,
                                           float FrameDeltaTime,
                                           const FAnimNotifyEventReference& EventReference) {
  elapsedTime += FrameDeltaTime;
  if (AnimationProgressComponent == nullptr) {
    return;
  }
  AnimationProgressComponent->Progress = FMath::Clamp(elapsedTime / Animation->GetPlayLength(), 0, 1);
  AnimationProgressComponent->ElapsedTime = elapsedTime;
};

void UAnimNotifyState_Progress::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                          UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference) {

  if (AnimationProgressComponent == nullptr) {
    return;
  }

  AnimationProgressComponent->Progress = 1.0F;
  AnimationProgressComponent->ElapsedTime = elapsedTime;
  AnimationProgressComponent->bInProgress = false;
};
