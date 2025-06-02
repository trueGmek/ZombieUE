#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"

#include "ZombieAIController.generated.h"

UCLASS()
class ZOMBIE_API AZombieAIController : public AAIController {

  GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, Category = "AI")
  UBehaviorTree *BehaviorTree;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  UAIPerceptionComponent *AIperceptionComponent;

  AZombieAIController();

protected:
  void OnPossess(APawn *InPawn) override;
  void BeginPlay() override;
};
