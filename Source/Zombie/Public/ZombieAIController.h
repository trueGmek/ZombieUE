#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
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

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  TObjectPtr<UBehaviorTree> ChaseBehaviorTree;

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

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName IsHitBlackboardKey{"IsHit"};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  FGameplayTag ChaseInjectionTag;

protected:
  FTimerHandle SeenPlayerTimerHandle;

public:
  AZombieAIController();

protected:
  void OnPossess(APawn* InPawn) override;
  virtual void BeginPlay() override;
  virtual void SetUpBehaviorTree();

private:
  UFUNCTION()
  void UpdatePerception(AActor* Actor, FAIStimulus Stimulus);

  UFUNCTION()
  void HandleTakeAnyDamage();

  void LoseEnemyReference() const;
};
