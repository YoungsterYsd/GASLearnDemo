// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GLDGameplayAbility_Jump.h"
#include "GLDCharacterBase.h"

UGLDGameplayAbility_Jump::UGLDGameplayAbility_Jump(const FObjectInitializer& ObjInitor):Super(ObjInitor)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGLDGameplayAbility_Jump::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags ,
	const FGameplayTagContainer* TargetTags,
	OUT FGameplayTagContainer* OptionalRelevantTags) const 
{
	//获取技能释放对象
	if( !ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}
	const AGLDCharacterBase* Character = Cast<AGLDCharacterBase>(ActorInfo->AvatarActor.Get());

	if(!Character ||!Character->CanJump())
	{
		return false;
	}
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return true;

}

void UGLDGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	CharacterJumpStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGLDGameplayAbility_Jump::CharacterJumpStart()
{
	if (AGLDCharacterBase* Character = Cast<AGLDCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		if (Character->IsLocallyControlled() && !Character->bPressedJump)
		{
			Character->UnCrouch();
			Character->Jump();
		}
	}
}

void UGLDGameplayAbility_Jump::CharacterJumpStop()
{
	if (AGLDCharacterBase* Character = Cast<AGLDCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		if (Character->IsLocallyControlled() && Character->bPressedJump)
		{
			Character->StopJumping();
		}
	}
}


