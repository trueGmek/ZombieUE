#include "ZombieRootMotionCharacter.h"
#include "Components/RootMotionNavigationComponent.h"
#include "HAL/Platform.h"

AZombieRootMotionCharacter::AZombieRootMotionCharacter() {
  RootMotionNavigationComponent =
      CreateDefaultSubobject<URootMotionNavigationComponent>(TEXT("RootMotionNavigationComponent"));
}
