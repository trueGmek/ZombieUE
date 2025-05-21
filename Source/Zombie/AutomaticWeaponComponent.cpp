#include "AutomaticWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ZombieCharacter.h"
#include "ZombieProjectile.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"


bool UAutomaticWeaponComponent::AttachWeaponToPlayer(AZombieCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UAutomaticWeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	
	GLog->Log("Owner before attaching: " + GetOwner()->GetName());
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	GLog->Log("Owner after attaching: " + GetOwner()->GetName());

	BindInputs();

	return true;
}

void UAutomaticWeaponComponent::BindInputs()
{
	if (const auto* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,
			                                   &UAutomaticWeaponComponent::CacheFirePressed);

			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this,
			                                   &UAutomaticWeaponComponent::CacheFireReleased);
		}
	}
}


void UAutomaticWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LastFireTime += DeltaTime;

	if (bShouldFire && LastFireTime >= Cooldown)
	{
		Fire();
		LastFireTime = 0;
	}
}

void UAutomaticWeaponComponent::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITOR
	if (GetWorld())
	{
		FVector Start = GetComponentLocation();
		FVector End = Start + MuzzleOffset;

		DrawDebugSphere(GetWorld(), End, 8.f, 12, FColor::Red, false, 1.f); // 10s visible
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 2.f);
	}
#endif
}

void UAutomaticWeaponComponent::CacheFirePressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("PRESSED"));
	bShouldFire = true;
}

void UAutomaticWeaponComponent::CacheFireReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("RELEASED"));
	bShouldFire = false;
}

void UAutomaticWeaponComponent::Fire()
{
	ensureMsgf(Character->GetController(), TEXT("Character->GetController() == nullptr"));
	AController* const CharacterController = Character->GetController();
	UWorld* const World = GetWorld();

	GLog->Log("Character name: " + Character->GetName());
	GLog->Log("ControllerName name: " + CharacterController->GetName());
	
	ensureMsgf(ProjectileClass, TEXT("ProjectileClass != nullptr"));
	ensureMsgf(World, TEXT("World != nullptr"));
	
	APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
	
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
	GLog->Log("Owner name: " + GetOwner()->GetName());

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	World->SpawnActor<AZombieProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	DrawDebugSphere(GetWorld(), SpawnLocation, 8.f, 12, FColor::Red, false, 1.f);


	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
