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
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

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

void UAutomaticWeaponComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

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
	bShouldFire = true;
}

void UAutomaticWeaponComponent::CacheFireReleased()
{
	bShouldFire = false;
}


void UAutomaticWeaponComponent::Fire() const
{
	ensureMsgf(Character->GetController(), TEXT("Character->GetController() == nullptr"));
	AController* const CharacterController = Character->GetController();

	ensureMsgf(GetWorld(), TEXT("World != nullptr"));
	UWorld* const World = GetWorld();

	if (Type == EType::Projectile)
		SpawnProjectile(CharacterController, World);
	if (Type == EType::Hitscan)
		HitScan(CharacterController);


	ManageFX();
}

void UAutomaticWeaponComponent::ManageFX() const
{
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

void UAutomaticWeaponComponent::SpawnProjectile(AController* const CharacterController, UWorld* const World) const
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ensureMsgf(ProjectileClass, TEXT("ProjectileClass != nullptr"));
	World->SpawnActor<AZombieProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void UAutomaticWeaponComponent::HitScan(AController* const CharacterController) const
{
	// Get the player's view point
	FVector EyeLocation;
	FRotator EyeRotation;
	CharacterController->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	const FVector End = EyeLocation + EyeRotation.Vector() * 1000;

	FHitResult HitResult{};
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CharacterController);
	QueryParams.bTraceComplex = true;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, End, ECC_Visibility, QueryParams);

	if (bHit)
	{
		DrawDebugLine(GetWorld(), EyeLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), HitResult.Location, 8.0f, 12, FColor::Green, false, 1.0f);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, EyeRotation.Vector(), HitResult,
			                                   CharacterController, CharacterController, nullptr);
		}
	}
	else
	{
		// Debug line for miss
		DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Blue, false, 1.0f, 0, 1.0f);
	}
}
