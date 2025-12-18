// Copyright Epic Games, Inc. All Rights Reserved.

#include "GLDGameMode.h"

#include "GLDCharacter.h"
#include "GLDPlayerState.h"
#include "GLDPlayerController.h"

#include "GLDGameState.h"
#include "GLDHUD.h"
#include "UObject/ConstructorHelpers.h"

AGLDGameMode::AGLDGameMode()
{
	DefaultPawnClass = AGLDCharacter::StaticClass();

	GameStateClass = AGLDGameState::StaticClass();	
	PlayerStateClass = AGLDPlayerState::StaticClass();
	HUDClass = AGLDHUD::StaticClass();
	PlayerControllerClass = AGLDPlayerController::StaticClass();
}
