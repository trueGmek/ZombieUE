#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/BlackboardValuesSetter.h"
#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "UObject/ObjectMacros.h"
#include "ZombieCharacter.h"

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

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  UBlackboardValuesSetter* BlackboardValuesSetter;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  float FollowAfterLosingSightPeriod{3.0F}; // NOLINT
                                            //
  UPROPERTY(EditDefaultsOnly, Category = "AI")
  float DestroyAgentTimeDelay{5.0F}; // NOLINT

  UPROPERTY()
  UBlackboardComponent* BlackboardComponent;

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName EnemyBlackboardKey{"EnemyActor"};

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName LOSBlackboardKey{"HasLineOfSight"};

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName IsHitBlackboardKey{"IsHit"};

  UPROPERTY(EditDefaultsOnly, Category = "AI")
  FName IsDeadBlackobardKey{"IsDead"};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  FName LastKnownEnemyLocationKey{"LastKnownEnemyLocation"};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  FName TimeOfLastEnemySight{"TimeOfLastEnemySight"};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  FGameplayTag ChaseInjectionTag;

protected:
  FTimerHandle SeenPlayerTimerHandle;
  FTimerHandle DestroyAgentTimerHandle;

  UPROPERTY()
  TObjectPtr<AZombieCharacter> ZombieCharacter;

public:
  AZombieAIController();

protected:
  void OnPossess(APawn* InPawn) override;
  virtual void BeginPlay() override;
  virtual void SetUpBehaviorTree();

  UFUNCTION()
  void UpdatePerception(AActor* Actor, FAIStimulus Stimulus);

  UFUNCTION()
  void SetDamageBlackboardFlag();

  UFUNCTION()
  void HandleDeathLogic();

private:
  void LoseEnemyReference() const;
  void DestroyAgent();
};
