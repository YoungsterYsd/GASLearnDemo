// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/GLDActorComponentBase.h"
#include "NativeGameplayTags.h"
#include "GLDAttributeSetCharacter.h"
#include "GLDHealthComponent.generated.h"


/**
 * 
 */
class UGLDHealthComponent;
class UGLDAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLDHealthComp_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGLDHealthComp_AttributeChanged,
	UGLDHealthComponent*, HealthComponent,
	float, OldValue,
	float, NewValue,
	AActor*, Instigator);
UENUM(BlueprintType)
enum class EGLDDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GLD_Damage_Message);

UCLASS()
class GLD_API UGLDHealthComponent : public UGLDActorComponentBase
{
	GENERATED_BODY()
public:
	UGLDHealthComponent(const FObjectInitializer& ObjInitor);

	UFUNCTION(BlueprintPure, Category = "GLD|Health")
	static UGLDHealthComponent* GetHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGLDHealthComponent>() : nullptr); }
#pragma region Common
	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeWithAbilitySystem(UGLDAbilitySystemComponent* InASC);
	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	void UninitializeFromAbilitySystem();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UGLDAbilitySystemComponent> AbilitySystemComponent;
	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UGLDAttributeSetCharacter> HealthSet;
	virtual void OnUnregister() override;
	void ClearGameplayTags();
#pragma endregion

#pragma region DeathHandle
public:
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	EGLDDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "GLD|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EGLDDeathState::NotDead); }
	// Begins the death sequence for the owner.
	virtual void StartDeath();
	// Ends the death sequence for the owner.
	virtual void FinishDeath();
	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);
	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_DeathEvent OnDeathFinished;
protected:
	UFUNCTION()
	virtual void OnRep_DeathState(EGLDDeathState OldDeathState);
	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EGLDDeathState DeathState;
#pragma endregion

#pragma region AttrDef
public:
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetHealth() const { return (HealthSet ? HealthSet->GetHealth() : 0.0f);}

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetMaxHealth() const { return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f); };

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetEnergy() const { return (HealthSet ? HealthSet->GetEnergy() : 0.0f); }

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetMaxEnergy() const { return (HealthSet ? HealthSet->GetMaxEnergy() : 0.0f); }

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetHealthNormalized() const {return GetMaxHealth() > 0.0f ? GetHealth()/GetMaxHealth(): 0.0f;}

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "GLD|Health")
	float GetEnergyNormalized() const { return GetMaxEnergy() > 0.0f ? GetEnergy() / GetMaxEnergy() : 0.0f; };

	// Delegate fired when the health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_AttributeChanged OnMaxHealthChanged;

	// Delegate fired when the mana value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_AttributeChanged OnEnergyChanged;

	// Delegate fired when the max mana value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FGLDHealthComp_AttributeChanged OnMaxEnergyChanged;
protected:
	virtual void HandleHealthChanged(AActor* DamageInstigator,AActor* DamageCauser,const FGameplayEffectSpec* DamageEffectSpec,float DamageMagnitude, float OldValue,float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator,AActor* DamageCauser,const FGameplayEffectSpec* DamageEffectSpec,float DamageMagnitude,float OldValue,float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator,AActor* DamageCauser,const FGameplayEffectSpec* DamageEffectSpec,float DamageMagnitude,float OldValue,float NewValue);
	virtual void HandleEnergyChanged(AActor* DamageInstigator,AActor* DamageCauser,const FGameplayEffectSpec* DamageEffectSpec,float DamageMagnitude,float OldValue,float NewValue);
	virtual void HandleMaxEnergyChanged(AActor* DamageInstigator,AActor* DamageCauser,const FGameplayEffectSpec* DamageEffectSpec,float DamageMagnitude,float OldValue,float NewValue);
#pragma endregion
};
