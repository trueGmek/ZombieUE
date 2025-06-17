
#include "BTDecorator_IsInRange.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_IsInRange)

UBTDecorator_IsInRange::UBTDecorator_IsInRange(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

  INIT_DECORATOR_NODE_NOTIFY_FLAGS();
  NodeName = "Is in range";

  Actor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRange, Actor), AActor::StaticClass());
}

void UBTDecorator_IsInRange::InitializeFromAsset(UBehaviorTree& Asset) {

  Super::InitializeFromAsset(Asset);

  if (Asset.BlackboardAsset) {
    Actor.ResolveSelectedKey(*Asset.GetBlackboardAsset());
  }
}

FString UBTDecorator_IsInRange::GetStaticDescription() const {
  // basic info: result after time
  return FString::Printf(TEXT("%s: check if agent is %s cm from %s"), *Super::GetStaticDescription(), *Range.ToString(),
                         *Actor.SelectedKeyName.GetPlainNameString());
}

bool UBTDecorator_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

  UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
  AActor* agentCharacter = OwnerComp.GetAIOwner()->GetPawn();
  UObject* bbObject = blackboard->GetValue<UBlackboardKeyType_Object>(Actor.GetSelectedKeyID());
  AActor* targetActor = Cast<AActor>(bbObject);
  ensureMsgf(targetActor, TEXT("Couldn't convert %s to AActor"), *Actor.SelectedKeyName.GetPlainNameString());

  return FVector::DistSquared(agentCharacter->GetActorLocation(), targetActor->GetActorLocation()) <=
         FMath::Square(Range.GetValue(blackboard));
}
