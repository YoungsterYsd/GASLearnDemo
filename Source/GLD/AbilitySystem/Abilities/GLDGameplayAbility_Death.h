// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GLDGameplayAbility.h"
#include "GLDGameplayAbility_Death.generated.h"

/**
 * 
 */
class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
UCLASS()
class GLD_API UGLDGameplayAbility_Death : public UGLDGameplayAbility
{
	GENERATED_BODY()
public:
	UGLDGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	// Starts the death sequence.
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void StartDeath();

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void FinishDeath();

protected:

	// If enabled, the ability will automatically call StartDeath.  FinishDeath is always called when the ability ends if the death was started.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
	bool bAutoStartDeath;
};
