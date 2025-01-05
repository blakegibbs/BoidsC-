// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoidsAndAnimationGameMode.h"
#include "BoidsAndAnimationCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABoidsAndAnimationGameMode::ABoidsAndAnimationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
