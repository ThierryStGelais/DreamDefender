// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DreamDefender.h"
#include "DreamDefenderGameMode.h"
#include "DreamDefenderCharacter.h"

ADreamDefenderGameMode::ADreamDefenderGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
