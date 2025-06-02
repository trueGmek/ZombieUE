#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"

#include "UObject/ObjectMacros.h"
#include "ZombieAIController.generated.h"

UCLASS()
class ZOMBIE_API AZombieAIController : public AAIController {

  GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, Category = "AI")
  UBehaviorTree* BehaviorTree;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  UAIPerceptionComponent* AIperceptionComponent;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  float FollowAfterLosingSightPeriod{3.0F}; // NOLINT

  UPROPERTY()
  UBlackboardComponent* BlackboardComponent;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName EnemyBlackboardKey{"EnemyActor"};

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName LOSBlackboardKey{"HasLineOfSight"};

  AZombieAIController();

protected:
  FTimerHandle SeenPlayerTimerHandle;

  void OnPossess(APawn* InPawn) override;
  void BeginPlay() override;

private:
  UFUNCTION()
  void UpdatePerception(AActor* Actor, FAIStimulus Stimulus);

  void LoseEnemyReference() const;
};
