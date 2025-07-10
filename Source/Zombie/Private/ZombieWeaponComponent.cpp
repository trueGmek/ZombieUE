// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieWeaponComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "ZombieProjectile.h"

// Sets default values for this component's properties
UZombieWeaponComponent::UZombieWeaponComponent() {
  // Default offset from the character location for projectiles to spawn
  MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UZombieWeaponComponent::Fire() {
  if (Character == nullptr || Character->GetController() == nullptr) {
    return;
  }

  // Try and fire a projectile
  if (ProjectileClass != nullptr) {
    UWorld* const World = GetWorld();
    if (World != nullptr) {
      APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
      const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
      // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character
      // location to find the final muzzle position
      GLog->Log("Owner name: " + GetOwner()->GetName());
      const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

      // Set Spawn Collision Handling Override
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride =
          ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      // Spawn the projectile at the muzzle
      World->SpawnActor<AZombieProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }
  }

  // Try and play the sound if specified
  if (FireSound != nullptr) {
    UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
  }

  // Try and play a firing animation if specified
  if (FireAnimation != nullptr) {
    // Get the animation object for the arms mesh
    UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
    if (AnimInstance != nullptr) {
      AnimInstance->Montage_Play(FireAnimation, 1.f);
    }
  }
}

bool UZombieWeaponComponent::AttachWeapon(APlayerCharacter* TargetCharacter) {
  Character = TargetCharacter;

  // Check that the character is valid, and has no weapon component yet
  if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UZombieWeaponComponent>()) {
    return false;
  }

  // Attach the weapon to the First Person Character
  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  GLog->Log("Owner before attaching: " + GetOwner()->GetName());
  AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
  GLog->Log("Owner after attaching: " + GetOwner()->GetName());

  // Set up action bindings
  if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PlayerController->GetLocalPlayer())) {
      // Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when
      // using touch input
      Subsystem->AddMappingContext(FireMappingContext, 1);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent =
            Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
      // Fire
      EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
                                         &UZombieWeaponComponent::Fire);
    }
  }

  return true;
}

void UZombieWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  // ensure we have a character owner
  if (Character != nullptr) {
    // remove the input mapping context from the Player Controller
    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
              ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                  PlayerController->GetLocalPlayer())) {
        Subsystem->RemoveMappingContext(FireMappingContext);
      }
    }
  }

  // maintain the EndPlay call chain
  Super::EndPlay(EndPlayReason);
}

void UZombieWeaponComponent::BeginPlay() {
  Super::BeginPlay();
}
