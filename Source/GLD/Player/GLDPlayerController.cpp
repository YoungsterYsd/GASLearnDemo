// Copyright Epic Games, Inc. All Rights Reserved.


#include "GLDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "GLD.h"
#include "GLDAbilitySystemComponent.h"
#include "GLDPlayerState.h"
#include "GLDGameplayAbility.h"
#include "GLDCharacterBase.h"
#include "GLDHUD.h"
#include "AbilitySystem/GLDAbilitySystemComponent.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AGLDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogGLD, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AGLDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AGLDPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AGLDPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UGLDAbilitySystemComponent* GldASC = Cast<UGLDAbilitySystemComponent>(GetGLDAbilitySystemComponent()))
	{
		GldASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);	
}
AGLDPlayerState* AGLDPlayerController::GetGLDPlayerState() const 
{
	return CastChecked<AGLDPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}
UGLDAbilitySystemComponent* AGLDPlayerController::GetGLDAbilitySystemComponent()const
{
	AGLDCharacterBase* GldCharacter = Cast<AGLDCharacterBase>(GetPawn());
	if(GldCharacter)
	{
		return GldCharacter->GetGLDAbilitySystemComponent();
	}
	return nullptr;
}
AGLDHUD* AGLDPlayerController::GetGLDHUD()const
{
	return CastChecked<AGLDHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}
