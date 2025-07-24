#pragma once

#include "Components/RootMotionNavigationComponent.h"
#include "CoreMinimal.h"
#include "ZombieCharacter.h"

#include "ZombieRootMotionCharacter.generated.h"

UCLASS()
class ZOMBIE_API AZombieRootMotionCharacter : public AZombieCharacter {
  GENERATED_BODY()

public:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = NPC, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URootMotionNavigationComponent> RootMotionNavigationComponent;

  AZombieRootMotionCharacter();
};
