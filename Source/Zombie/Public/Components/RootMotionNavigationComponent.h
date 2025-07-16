#pragma once

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "UObject/ObjectPtr.h"
#include "Unix/UnixPlatform.h"
#include "ZombieAnimInstance.h"

#include "RootMotionNavigationComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
  Walk UMETA(DisplayName = "Walk"),
  Run UMETA(DisplayName = "Run"),
};

/*
 * URootMotionNavigationComponent is an component for finding paths with root motion based actors
 */
UCLASS()
class ZOMBIE_API URootMotionNavigationComponent : public UActorComponent {
  GENERATED_BODY()

protected:
  UPROPERTY()
  TObjectPtr<UZombieAnimInstance> ZombieAnimInstance;

  UPROPERTY()
  TObjectPtr<UNavigationSystemV1> NavigationSystem;

  UPROPERTY()
  const UNavigationPath* CachedPath;

  UPROPERTY(EditAnywhere, meta = (ToolTip = "Used in ProjectPointToNavMesh method"))
  FVector QueryExtent{100, 100, 100};

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
  bool bIsUsed;

public:
  URootMotionNavigationComponent();

  /*
   * Find a path from Owner's position to Destination
   */
  UFUNCTION()
  UNavigationPath* FindPathToDestination(const FVector& Destination);

  /*
   * Finds a path from Owner's position to a Target
   */
  UFUNCTION()
  UNavigationPath* FindPathToActor(AActor* Target);

  /*
   * Find a path from StartPosition to Destination
   */
  UFUNCTION()
  UNavigationPath* FindPathFromStartToDestination(const FVector& StartPosition, const FVector& Destination);

  /*
   * Find a path from StartPosition to a Target
   */
  UFUNCTION()
  UNavigationPath* FindPathFromStartToActor(const FVector& StartPosition, AActor* Target);

  /*
   *
   */
  // TODO: Documentation
  UFUNCTION()
  UNavigationPath* FindPathToBlackboardDestination(
      const FBlackboardKeySelector& Destination, const UBlackboardComponent* BlackboardComponent);

  /*
   * Starts the movement by setting animation flags
   */
  UFUNCTION()
  void StartMovement(EMovementType MovementType);

  /*
   * Stops the movement by setting animation flags
   */
  UFUNCTION()
  void StopMovement();

  /*
   * Finds
   */
  UFUNCTION()
  bool ProjectPointToNavMesh(FVector const& Inposition, FVector& OutPosition);

  FORCEINLINE bool GetUsed() const {
    return bIsUsed;
  }

  FORCEINLINE const UNavigationPath* GetLastPath() const {
    return CachedPath;
  }

protected:
  void BeginPlay() override;
};
