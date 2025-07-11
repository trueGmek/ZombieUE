#pragma once

#include "GameplayDebuggerCategory.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.h"

struct FDebugData {

  bool bIsMoving;
  float HP;
  FString DebugActorName;
  TArray<FVector> PathPoints;

  void Serialize(FArchive& Ar);
};

// TODO: Rename to FZombieDebuggerCategory_AI
class FZombieAIDebugger : public FGameplayDebuggerCategory {
public:
  static const FName CategoryName;
  static const int CategoryNumber{5};

protected:
  FDebugData DebugData;

private:
  static const FDebugData DefaultDebugData;

public:
  FZombieAIDebugger();

  static TSharedRef<FGameplayDebuggerCategory> MakeInstance();
  virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
  virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
  void DrawPath(UWorld* World) const;
  static FDebugData FetchDebugData(AZombieCharacter& Character);
};
