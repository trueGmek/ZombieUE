// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameMode.h"
#include "ZombieCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieGameMode::AZombieGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
