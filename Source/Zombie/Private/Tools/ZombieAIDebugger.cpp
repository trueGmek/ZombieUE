#include "Tools/ZombieAIDebugger.h"
#include "Engine/Engine.h"
#include "GameplayDebugger.h"
#include "HAL/Platform.h"
#include "UObject/NameTypes.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.h"

const FName FZombieAIDebugger::CategoryName{TEXT("Zombie AI")};

FZombieAIDebugger::FZombieAIDebugger() {
  bShowOnlyWithDebugActor = false;
  SetDataPackReplication<FDebugData>(&DebugData);
}

TSharedRef<FGameplayDebuggerCategory> FZombieAIDebugger::MakeInstance() {
  return MakeShareable(new FZombieAIDebugger());
}

void FZombieAIDebugger::CollectData(APlayerController* OwnerPC, AActor* DebugActor) {
  APawn* DebugPawn = Cast<APawn>(DebugActor);
  if (DebugPawn == nullptr) {
    DebugData.DebugActorName = TEXT("None");
    DebugData.HP = -1;
    return;
  }

  AZombieAIController* ZombieController = Cast<AZombieAIController>(DebugPawn->GetController());
  if (ZombieController == nullptr) {
    DebugData.DebugActorName = TEXT("None");
    DebugData.HP = -1;
    return;
  }

  AZombieCharacter* Pawn = Cast<AZombieCharacter>(ZombieController->GetPawn());
  if (Pawn != nullptr) {
    DebugData.DebugActorName = DebugActor->GetFullName();
    DebugData.HP = Pawn->HealthComponent->CurrentHealth;
  } else {
    DebugData.DebugActorName = TEXT("None");
    DebugData.HP = -1;
  }
}

void FZombieAIDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) {
  CanvasContext.Printf(FColor::White, TEXT("--- MY CUSTOM AI DEBUG ---"));
  CanvasContext.Printf(FColor::Green, TEXT("Name: {yellow}%s"), *DebugData.DebugActorName);
  CanvasContext.Printf(FColor::Green, TEXT("HP: %f"), DebugData.HP);
}

void FDebugData::Serialize(FArchive& Ar) {
  Ar << HP;
  Ar << DebugActorName;
}
