#include "Components/BlackboardValuesSetter.h"
#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBlackboardValuesSetter::SetBlackboardKeySelectorFilters() {
  if (GetHitAnimationKey.SelectedKeyName == NAME_None) {
    GetHitAnimationKey.AddObjectFilter(
        this, GET_MEMBER_NAME_CHECKED(UBlackboardValuesSetter, GetHitAnimationKey), UAnimMontage::StaticClass());
  }
}

void UBlackboardValuesSetter::PostInitProperties() {
  Super::PostInitProperties();
  SetBlackboardKeySelectorFilters();
  TryResolvingKey(GetHitAnimationKey);
}

void UBlackboardValuesSetter::PostLoad() {
  Super::PostLoad();
  SetBlackboardKeySelectorFilters();
  TryResolvingKey(GetHitAnimationKey);
}

void UBlackboardValuesSetter::SetValues(UBlackboardComponent& BlackboardComponent) {
  if (GetHitAnimationKey.SelectedKeyName != NAME_None) {
    BlackboardComponent.SetValue<UBlackboardKeyType_Object>(GetHitAnimationKey.GetSelectedKeyID(), GetHitAnimMontage);
  }
}

void UBlackboardValuesSetter::BeginPlay() {
  Super::BeginPlay();

  AAIController* AIController = Cast<AAIController>(GetOwner());
  if (AIController == nullptr) {
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn != nullptr) {
      AIController = Cast<AAIController>(OwnerPawn->GetController());
    }
  }
  SetValues(*AIController->GetBlackboardComponent());
}

void UBlackboardValuesSetter::TryResolvingKey(FBlackboardKeySelector& key) const {
  if (BBAsset != nullptr) {
    key.ResolveSelectedKey(*BBAsset);
  }
}

#if WITH_EDITOR
void UBlackboardValuesSetter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
  TryResolvingKey(GetHitAnimationKey);
}
#endif // WITH_EDITOR
