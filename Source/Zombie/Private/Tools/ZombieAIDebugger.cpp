#include "Tools/ZombieAIDebugger.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "GameplayDebugger.h"
#include "HAL/Platform.h"
#include "NavigationSystem.h"
#include "UObject/NameTypes.h"
#include "ZombieAIController.h"
#include "ZombieCharacter.h"

void FDebugData::Serialize(FArchive& Ar) {
  Ar << bIsMoving;
  Ar << HP;
  Ar << DebugActorName;
  Ar << PathPoints;
}

const FName FZombieAIDebugger::CategoryName{TEXT("Zombie AI")};
const FDebugData FZombieAIDebugger::DefaultDebugData{false, -1, TEXT("NONE"), {}};

FZombieAIDebugger::FZombieAIDebugger() {
  bShowOnlyWithDebugActor = false;
  SetDataPackReplication<FDebugData>(&DebugData);
}

TSharedRef<FGameplayDebuggerCategory> FZombieAIDebugger::MakeInstance() {
  return MakeShareable(new FZombieAIDebugger());
}

void FZombieAIDebugger::CollectData(APlayerController* OwnerPC, AActor* DebugActor) {
  APawn* DebugPawn = Cast<APawn>(DebugActor);
  DebugData = DefaultDebugData;

  if (DebugPawn == nullptr) {
    return;
  }

  AZombieAIController* ZombieController = Cast<AZombieAIController>(DebugPawn->GetController());
  if (ZombieController == nullptr) {
    return;
  }

  AZombieCharacter* Pawn = Cast<AZombieCharacter>(ZombieController->GetPawn());
  if (Pawn != nullptr) {
    DebugData = FetchDebugData(*Pawn);
  }
}

FDebugData FZombieAIDebugger::FetchDebugData(AZombieCharacter& Character) {
  auto RMNavigationComponent = Character.RootMotionNavigationComponent;

  if (RMNavigationComponent == nullptr || RMNavigationComponent->GetLastPath() == nullptr ||
      Character.HealthComponent == nullptr) {
    return DefaultDebugData;
  }

  return {
      RMNavigationComponent->GetUsed(),
      Character.HealthComponent->CurrentHealth,
      Character.GetFullName(),
      RMNavigationComponent->GetLastPath()->PathPoints};
}

void FZombieAIDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) {
  CanvasContext.Printf(FColor::White, TEXT("--- MY CUSTOM AI DEBUG ---"));
  CanvasContext.Printf(FColor::Green, TEXT("Name: {yellow}%s"), *DebugData.DebugActorName);
  CanvasContext.Printf(FColor::Green, TEXT("HP: %f"), DebugData.HP);

  if (DebugData.bIsMoving) {
    DrawPath(CanvasContext.GetWorld());
  }
}
void FZombieAIDebugger::DrawPath(UWorld* World) const {
  const FColor InactiveColor(100, 100, 100);
  const FColor PathColor(192, 192, 192);
  const FColor PathGoalColor(255, 255, 255);
  const int32 NumPathVerts = DebugData.PathPoints.Num();
  for (int32 Idx = 0; Idx < NumPathVerts; Idx++) {
    const FVector PathPoint = DebugData.PathPoints[Idx] + NavigationDebugDrawing::PathOffset;
    DrawDebugSolidBox(
        World,
        PathPoint,
        NavigationDebugDrawing::PathNodeBoxExtent,
        PathColor); // TODO: Idx < DataPack.NextPathPointIndex ? InactiveColor : PathColor
  }
  for (int32 Idx = 1; Idx < NumPathVerts; Idx++) {
    const FVector P0 = DebugData.PathPoints[Idx - 1] + NavigationDebugDrawing::PathOffset;
    const FVector P1 = DebugData.PathPoints[Idx] + NavigationDebugDrawing::PathOffset;

    DrawDebugLine(
        World,
        P0,
        P1,
        PathColor, // TODO: Idx < DataPack.NextPathPointIndex ? InactiveColor : PathColor,
        false,
        -1.0F,
        0,
        NavigationDebugDrawing::PathLineThickness);
  }

  // TODO:
  //  if (NumPathVerts && DataPack.bPathHasGoalActor) {
  //    const FVector P0 = PathDataPack.PathPoints.Last() + NavigationDebugDrawing::PathOffset;
  //    const FVector P1 = DataPack.PathGoalLocation + NavigationDebugDrawing::PathOffset;
  //
  //    DrawDebugLine(World, P0, P1, PathGoalColor, false, -1.0f, 0, NavigationDebugDrawing::PathLineThickness);
  //  }
}
