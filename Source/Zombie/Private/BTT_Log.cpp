// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_Log.h"

UBTT_Log::UBTT_Log() {
  bNotifyTick = false;
  bNotifyTaskFinished = true;
  bCreateNodeInstance = false;
  NodeName = "Log a message";
}

EBTNodeResult::Type UBTT_Log::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

  if (OwnerComp.GetAIOwner() == nullptr) {
    return EBTNodeResult::Failed;
  }

  UE_LOG(LogTemp, Log, TEXT("%s"), *this->message);

  return EBTNodeResult::Succeeded;
}

FString UBTT_Log::GetStaticDescription() const {
  return FString::Printf(TEXT("Log: %s"), *message);
}
