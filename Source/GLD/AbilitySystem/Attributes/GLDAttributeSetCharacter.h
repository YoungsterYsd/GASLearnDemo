// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GLDAttributeSet.h"
#include "GLDAttributeSetCharacter.generated.h"

/**
 * 
 */
#define IMPLEMENT_ONREP(AttributeName)\
void UGLDAttributeSetCharacter::OnRep_##AttributeName(const FGameplayAttributeData& OldValue) \
{ GAMEPLAYATTRIBUTE_REPNOTIFY(UGLDAttributeSetCharacter, AttributeName, OldValue); }

UCLASS()
class GLD_API UGLDAttributeSetCharacter : public UGLDAttributeSet 
{
	GENERATED_BODY()
public:
	UGLDAttributeSetCharacter();
    // Replication setup
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

public:
	//属性变化事件委托
    mutable FGLDAttributeEvent OnHealthChanged;
    mutable FGLDAttributeEvent OnMaxHealthChanged;
    mutable FGLDAttributeEvent OnOutOfHealth;

    mutable FGLDAttributeEvent OnEnergyChanged;
    mutable FGLDAttributeEvent OnMaxEnergyChanged;
private:
    bool bOutOfHealth;
	float MaxHealthBeforeChange;
	float HealthBeforeChange;
    float MaxEnergyBeforeChange;

#pragma region  Attribute Definitions
public:
    //属性定义
    // 等级
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Level);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, MaxLevel);

    // 经验
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Experience);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, MaxExperience);

    // 血量
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Health);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, MaxHealth);

    // 能量
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Energy);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, MaxEnergy);

    // 攻击、防御、防御穿透
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Attack);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Defense);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, DefensePenetration);

    // 暴击相关
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, CriticalRate);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, CriticalDamage);

    // 伤害加成和减免
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, DamageBonus);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, DamageReduction);

    // 固定伤害加成和减免
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, FlatDamageBonus);
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, FlatDamageReduction);

    // 战斗续航
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Sustain);

    //
    ATTRIBUTE_ACCESSORS(UGLDAttributeSetCharacter, Damage);
protected:
    // RepNotify 通知函数
    UFUNCTION()
    virtual void OnRep_Level(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_MaxLevel(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Experience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_MaxExperience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Energy(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_DefensePenetration(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_CriticalRate(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_CriticalDamage(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_DamageBonus(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_FlatDamageBonus(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_FlatDamageReduction(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Sustain(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);

public://作用域私有的情况下，计算方法没办法读到，如果做网络交互，应该私有化，但不是目前该考虑的事情。
    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Level", ReplicatedUsing = OnRep_Level, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Level;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Level", ReplicatedUsing = OnRep_MaxLevel, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Experience", ReplicatedUsing = OnRep_Experience, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Experience;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Experience", ReplicatedUsing = OnRep_MaxExperience, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxExperience;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Health", ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Health", ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Energy", ReplicatedUsing = OnRep_Energy, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Energy;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Energy", ReplicatedUsing = OnRep_MaxEnergy, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxEnergy;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Combat", ReplicatedUsing = OnRep_Attack, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Attack;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Combat", ReplicatedUsing = OnRep_Defense, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Defense;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Combat", ReplicatedUsing = OnRep_DefensePenetration, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData DefensePenetration;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Critical", ReplicatedUsing = OnRep_CriticalRate, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData CriticalRate;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Critical", ReplicatedUsing = OnRep_CriticalDamage, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData CriticalDamage;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|DamageBonus", ReplicatedUsing = OnRep_DamageBonus, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData DamageBonus;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|DamageReduction", ReplicatedUsing = OnRep_DamageReduction, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData DamageReduction;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|FlatDamageBonus", ReplicatedUsing = OnRep_FlatDamageBonus, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData FlatDamageBonus;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|FlatDamageReduction", ReplicatedUsing = OnRep_FlatDamageReduction, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData FlatDamageReduction;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Sustain", ReplicatedUsing = OnRep_Sustain, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Sustain;

    UPROPERTY(BlueprintReadOnly, Category = "Attribute|Damage", ReplicatedUsing = OnRep_Damage, Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Damage;
    #pragma endregion
};
