// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GLDHealthComponent.h"
#include "GLDLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "GLDAbilitySystemComponent.h"
#include "GLDAttributeSetCharacter.h"
#include "GLDGameplayTag.h"
#include "GLDGameplayAbility_Death.h"

UGLDHealthComponent::UGLDHealthComponent(const FObjectInitializer& ObjInitor):Super(ObjInitor)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	DeathState = EGLDDeathState::NotDead;

}

void UGLDHealthComponent::InitializeWithAbilitySystem(UGLDAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);
	//初始化时应该为空
	if (AbilitySystemComponent)
	{
		UE_LOG(LogGLDTest, Error, TEXT("HealthComponent: Health component for owner [%s] has already been initialized with an ability system."),*GetNameSafe(Owner));
		return;
	}
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogGLDTest, Error, TEXT("GLDHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."),*GetNameSafe(Owner));
		return;
	}
	HealthSet = AbilitySystemComponent->GetSet<UGLDAttributeSetCharacter>();
	if (!HealthSet)
	{
		UE_LOG(LogGLDTest, Error, TEXT("LyraHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."),
			*GetNameSafe(Owner));
		return;
	}
	// Register to listen for attribute changes.
	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
	HealthSet->OnEnergyChanged.AddUObject(this, &ThisClass::HandleEnergyChanged);
	HealthSet->OnMaxEnergyChanged.AddUObject(this, &ThisClass::HandleMaxEnergyChanged);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(UGLDAttributeSetCharacter::GetHealthAttribute(), HealthSet->GetMaxHealth());
	ClearGameplayTags();
	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnEnergyChanged.Broadcast(this, HealthSet->GetEnergy(), HealthSet->GetEnergy(), nullptr);
	OnMaxEnergyChanged.Broadcast(this, HealthSet->GetMaxEnergy(), HealthSet->GetMaxEnergy(), nullptr);
}

void UGLDHealthComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();
	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}
	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

void UGLDHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGLDHealthComponent, DeathState);
}

void UGLDHealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UGLDHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(GLDGameplayTag::Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(GLDGameplayTag::Status_Death_Dead, 0);
	}
}

void UGLDHealthComponent::StartDeath()
{
	if (DeathState != EGLDDeathState::NotDead)
	{
		return;
	}
	DeathState = EGLDDeathState::DeathStarted;
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(GLDGameplayTag::Status_Death_Dying, 1);
	}
	AActor* Owner = GetOwner();
	check(Owner);
	OnDeathStarted.Broadcast(Owner);
	Owner->ForceNetUpdate();
}

void UGLDHealthComponent::FinishDeath()
{
	if (DeathState != EGLDDeathState::DeathStarted)
	{
		return;
	}
	DeathState = EGLDDeathState::DeathFinished;
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(GLDGameplayTag::Status_Death_Dead, 1);
	}
	AActor* Owner = GetOwner();
	check(Owner);
	OnDeathFinished.Broadcast(Owner);
	Owner->ForceNetUpdate();
}
void UGLDHealthComponent::OnRep_DeathState(EGLDDeathState OldDeathState)
{
	//死亡判定的校验
	const EGLDDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogGLDTest, Warning, TEXT("GLDHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}
	if (OldDeathState == EGLDDeathState::NotDead)
	{
		if (NewDeathState == EGLDDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EGLDDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogGLDTest, Error, TEXT("GLDHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDeathState == EGLDDeathState::DeathStarted)
	{

		if (NewDeathState == EGLDDeathState::DeathFinished)
		{
			FinishDeath();
		}
		else
		{
			UE_LOG(LogGLDTest, Error, TEXT("GLDHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	ensureMsgf((DeathState == NewDeathState), TEXT("GLDHealthComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
}
//应用死亡效果并自毁
void UGLDHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	/*if ((DeathState == EGLDDeathState::NotDead) && AbilitySystemComponent)
	{
		const TSubclassOf<UGameplayEffect> DamageGE = UGLDAssetManager::GetSubclass(UGLDGameData::Get().DamageGameplayEffect_SetByCaller);
		if (!DamageGE)
		{
			UE_LOG(LogGLDTest, Error, TEXT("LyraHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to find gameplay effect [%s]."),*GetNameSafe(GetOwner()), *UGLDGameData::Get().DamageGameplayEffect_SetByCaller.GetAssetName());
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		if (!Spec)
		{
			UE_LOG(LogGLDTest, Error, TEXT("LyraHealthComponent: DamageSelfDestruct failed for owner [%s]. Unable to make outgoing spec for [%s]."), *GetNameSafe(GetOwner()), *GetNameSafe(DamageGE));
			return;
		}
		Spec->AddDynamicAssetTag(TAG_Gameplay_DamageSelfDestruct);
		if (bFellOutOfWorld)
		{
			Spec->AddDynamicAssetTag(TAG_Gameplay_FellOutOfWorld);
		}

		const float DamageAmount = GetMaxHealth();

		Spec->SetSetByCallerMagnitude(GLDGameplayTag::SetByCaller_Damage, DamageAmount);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
	}*/
}


void UGLDHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGLDHealthComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGLDHealthComponent::HandleEnergyChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnEnergyChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGLDHealthComponent::HandleMaxEnergyChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxEnergyChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UGLDHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	//需要在服务器处理
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = GLDGameplayTag::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec->Def;
			Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;
			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
#endif // #if WITH_SERVER_CODE
}