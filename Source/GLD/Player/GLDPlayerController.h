// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GLDPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

class UGLDAbilitySystemComponent;
class AGLDPlayerState;
class AGLDHUD;

class UGLDNumberPopComponent_UMG;
class UGLDNumberPopComponent;
/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS()
class AGLDPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGLDPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UFUNCTION(BlueprintCallable, Category = "GLDRPG|PlayerController")
	AGLDPlayerState* GetGLDPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "GLDRPG|PlayerController")
	UGLDAbilitySystemComponent* GetGLDAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "GLDRPG|PlayerController")
	AGLDHUD* GetGLDHUD() const;
	UFUNCTION(BlueprintCallable, Category = "GLDRPG|PlayerController")
	UGLDNumberPopComponent_UMG* GetGLDNumberPopComponent()const;
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;
	UPROPERTY(EditAnywhere)
	UGLDNumberPopComponent_UMG* NumberPopComp;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;
};
