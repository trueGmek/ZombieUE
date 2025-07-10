#pragma once

#include "GameplayDebuggerCategory.h"
#include "ZombieAIController.h"

struct FDebugData {
  float HP;
  FString DebugActorName;

  void Serialize(FArchive& Ar);
};

class FZombieAIDebugger : public FGameplayDebuggerCategory {
public:
  FZombieAIDebugger();

  static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
  virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
  virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

  static const FName CategoryName;
  static const int CategoryNumber{5};

protected:
  FDebugData DebugData;
};
