// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GLDGameplayAbility.h"
#include "GLDGameplayAbility_Jump.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * 
 */
UCLASS(Abstract)
class GLD_API UGLDGameplayAbility_Jump : public UGLDGameplayAbility
{
	GENERATED_BODY()
public:
	UGLDGameplayAbility_Jump(const FObjectInitializer& ObjInitor = FObjectInitializer::Get());


protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, 
		bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "GLD|Ability|Jump")
	void CharacterJumpStart();
	UFUNCTION(BlueprintCallable, Category = "GLD|Ability|Jump")
	void CharacterJumpStop();
	
};
