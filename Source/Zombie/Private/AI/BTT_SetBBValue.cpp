#include "AI/BTT_SetBBValue.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieCharacter.h"

UBTT_SetBBValue::UBTT_SetBBValue() {
  bNotifyTick = false;
  bCreateNodeInstance = false;

  Key.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_SetBBValue, Key));
}

void UBTT_SetBBValue::InitializeFromAsset(UBehaviorTree& Asset) {

  Super::InitializeFromAsset(Asset);

  if (Asset.BlackboardAsset) {
    Key.ResolveSelectedKey(*Asset.BlackboardAsset.Get());
  }
}

FString UBTT_SetBBValue::GetStaticDescription() const {
  return FString::Printf(TEXT("%s: Setting value to %s"), *Super::GetStaticDescription(),
                         *Key.SelectedKeyName.GetPlainNameString());
};

EBTNodeResult::Type UBTT_SetBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
  if (OwnerComp.GetAIOwner() == nullptr || Blackboard == nullptr) {
    return EBTNodeResult::Failed;
  }

  Blackboard->SetValue<UBlackboardKeyType_Bool>(Key.GetSelectedKeyID(), bValue);
  return EBTNodeResult::Succeeded;
}
