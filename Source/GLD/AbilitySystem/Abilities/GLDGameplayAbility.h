// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GLDPlayerController.h"
#include "GLDGameplayAbility.generated.h"

/**
 * 
 */

class UGLDAbilitySystemComponent;
class AGLDCharacterBase;
UCLASS(Abstract, HideCategories = Input)
class GLD_API UGLDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public :
	UGLDGameplayAbility(const FObjectInitializer& ObjInitor = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "GLD|Ability")
	UGLDAbilitySystemComponent* GetGLDOwningAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GLD|Ability")
	AGLDCharacterBase* GetGLDOwningCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GLD|Ability")
	AGLDPlayerController* GetGLDOwningPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GLD|Ability")
	AController* GetGLDOwningControllerFromActorInfo() const;

	
};
