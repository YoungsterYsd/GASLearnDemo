// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GLDAttributeSetCharacter.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GLDAbilitySystemComponent.h"

UGLDAttributeSetCharacter::UGLDAttributeSetCharacter()
{
    InitLevel(1.f);
    InitMaxLevel(100.f);

    InitExperience(0.f);
    InitMaxExperience(100.f);

    InitHealth(100.f);
    InitMaxHealth(100.f);
    InitEnergy(50.f);
    InitMaxEnergy(100.f);

    InitAttack(10.f);
    InitDefense(5.f);
    InitDefensePenetration(0.f);

    InitCriticalRate(0.1f);       // 10%
    InitCriticalDamage(1.5f);     // 150%

    InitDamageBonus(0.f);
    InitDamageReduction(0.f);

    InitFlatDamageBonus(0.f);
    InitFlatDamageReduction(0.f);

    InitSustain(0.f);

	bOutOfHealth = false;
    MaxHealthBeforeChange = 0.f;
    HealthBeforeChange = 0.f;
    MaxEnergyBeforeChange = 0.f;
}

void UGLDAttributeSetCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 使用REPNOTIFY_ALWAYS确保OnRep通知被始终调用
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Level, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, MaxLevel, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Experience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, MaxExperience, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, MaxHealth, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Energy, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, MaxEnergy, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Attack, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Defense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, DefensePenetration, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, CriticalRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, CriticalDamage, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, DamageBonus, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, DamageReduction, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, FlatDamageBonus, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, FlatDamageReduction, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Sustain, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGLDAttributeSetCharacter, Damage, COND_None, REPNOTIFY_Always);
}

bool UGLDAttributeSetCharacter::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	// 在属性被修改前保存当前血量和最大血量
	Super::PreGameplayEffectExecute(Data);
    MaxHealthBeforeChange = GetMaxHealth();
    HealthBeforeChange = GetHealth();
	MaxEnergyBeforeChange = GetMaxEnergy();
    return true;
}

void UGLDAttributeSetCharacter::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    // 检测是否掉血
    
    const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
    AActor* Instigator = EffectContext.GetOriginalInstigator();
    AActor* Causer = EffectContext.GetEffectCauser();

    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.f, GetMaxHealth()));
        SetDamage(0.0f);
    }
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
        // Notify on any requested max health changes
        OnMaxHealthChanged.Broadcast(Instigator,Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude,MaxHealthBeforeChange, GetMaxHealth());
        if(GetHealth() > GetMaxHealth())
        {
            SetHealth(GetMaxHealth());
		}
    }
    else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
    {
        // Clamp and fall into out of health handling below
        SetEnergy(FMath::Clamp(GetEnergy(),0,GetMaxEnergy()));
    }
    else if (Data.EvaluatedData.Attribute == GetMaxEnergyAttribute())
    {
        // Notify on any requested max Energt changes
        OnMaxEnergyChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxEnergyBeforeChange, GetMaxEnergy());
        if(GetEnergy() > GetMaxEnergy())
        {
            SetEnergy(GetMaxEnergy());
		}
    }

    // If health has actually changed activate callbacks
    if (GetHealth() != HealthBeforeChange)
    {
        OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeChange, GetHealth());
    }

    if ((GetHealth() <= 0.0f) && !bOutOfHealth)
    {
        OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeChange, GetHealth());
    }

    // Check health again in case an event above changed it.
    bOutOfHealth = (GetHealth() <= 0.0f);

}

void UGLDAttributeSetCharacter::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	return Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UGLDAttributeSetCharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	return Super::PreAttributeChange(Attribute, NewValue);
}

void UGLDAttributeSetCharacter::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UGLDAttributeSetCharacter::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGLDAttributeSetCharacter, Health, OldValue);
    const float CurrentHealth = GetHealth();
    const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
    OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
    if (!bOutOfHealth && CurrentHealth <= 0.0f)
    {
        OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
    }
    bOutOfHealth = (CurrentHealth <= 0.0f);
}

void UGLDAttributeSetCharacter::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGLDAttributeSetCharacter, MaxHealth, OldValue);
    OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxHealth());

}

void UGLDAttributeSetCharacter::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGLDAttributeSetCharacter, Energy, OldValue);

    const float CurrentMana = GetEnergy();
    const float EstimatedMagnitude = CurrentMana - OldValue.GetCurrentValue();

    OnEnergyChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentMana);
}

void UGLDAttributeSetCharacter::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGLDAttributeSetCharacter, MaxEnergy, OldValue);

    OnMaxEnergyChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxEnergy() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxEnergy());
}


//使用批量定义宏来实现简单OnRep函数
IMPLEMENT_ONREP(Level)
IMPLEMENT_ONREP(MaxLevel)
IMPLEMENT_ONREP(Experience)
IMPLEMENT_ONREP(MaxExperience)
IMPLEMENT_ONREP(Attack)
IMPLEMENT_ONREP(Defense)
IMPLEMENT_ONREP(DefensePenetration)
IMPLEMENT_ONREP(CriticalRate)
IMPLEMENT_ONREP(CriticalDamage) 
IMPLEMENT_ONREP(DamageBonus) 
IMPLEMENT_ONREP(DamageReduction)
IMPLEMENT_ONREP(FlatDamageBonus) 
IMPLEMENT_ONREP(FlatDamageReduction) 
IMPLEMENT_ONREP(Sustain)
IMPLEMENT_ONREP(Damage)