// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GLDGameplayAbility.h"
#include "GLDCharacterBase.h"
#include "GLDPlayerController.h"
#include "AbilitySystem/GLDAbilitySystemComponent.h"

UGLDGameplayAbility::UGLDGameplayAbility(const FObjectInitializer& ObjInitor) :Super(ObjInitor)
{

	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

}

UGLDAbilitySystemComponent* UGLDGameplayAbility::GetGLDOwningAbilitySystemComponentFromActorInfo() const
{
	if (CurrentActorInfo? Cast<UGLDAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()): nullptr)
	{
		return Cast<UGLDAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
	}

	return nullptr;
}

AGLDCharacterBase* UGLDGameplayAbility::GetGLDOwningCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AGLDCharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

AGLDPlayerController* UGLDGameplayAbility::GetGLDOwningPlayerControllerFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AGLDPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr;
}

AController* UGLDGameplayAbility::GetGLDOwningControllerFromActorInfo() const
{
	
	if(CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		AActor* TestActor = CurrentActorInfo->AvatarActor.Get();
		while (TestActor)
		{
			if (AController* Controller = Cast<AController>(TestActor))
			{
				return Controller;
			}
			if(APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}
			TestActor = TestActor->GetOwner();
		}
	}
	return nullptr;
}
