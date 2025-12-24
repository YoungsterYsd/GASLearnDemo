// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GLDGameplayAbility_Death.h"
#include "GLDGameplayAbility.h"
#include "GLDAbilitySystemComponent.h"
#include "GLDHealthComponent.h"
#include "GLDGameplayTag.h"
#include "GLDLogChannels.h"

UGLDGameplayAbility_Death::UGLDGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
{
	//每个角色死一次
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//服务器可调用
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		//由死亡事件的Tag调用
		TriggerData.TriggerTag = GLDGameplayTag::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGLDGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//获取ASC，激活死亡，关闭其它能力，带有Tag的能力免疫，本能力不可取消
	check(ActorInfo);
	UGLDAbilitySystemComponent* GLDASC = CastChecked<UGLDAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(GLDGameplayTag::Ability_Behavior_SurvivesDeath);
	// Cancel all abilities and block others from starting.
	GLDASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
	SetCanBeCanceled(false);
	if (bAutoStartDeath)
	{
		StartDeath();
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGLDGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);
	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGLDGameplayAbility_Death::StartDeath()
{
	//调用HealthCompnent的
	if (UGLDHealthComponent* HealthComponent = UGLDHealthComponent::GetHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EGLDDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}
void UGLDGameplayAbility_Death::FinishDeath()
{
	if (UGLDHealthComponent* HealthComponent = UGLDHealthComponent::GetHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EGLDDeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}