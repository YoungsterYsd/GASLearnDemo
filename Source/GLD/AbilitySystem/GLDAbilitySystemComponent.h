// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GLDAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UGLDAbilitySystemComponent(const FObjectInitializer& objInitor = FObjectInitializer::Get());
	
	void AbilityInputPressed(const FGameplayTag&  InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused) ;
	void ClearAbilityInput();
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
