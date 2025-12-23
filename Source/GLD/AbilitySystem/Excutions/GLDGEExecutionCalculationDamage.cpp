// Fill out your copyright notice in the Description page of Project Settings.

#include "GLDGEExecutionCalculationDamage.h"
#include "GLDAttributeSetCharacter.h"
// 以结构体封装属性捕获定义，统一管理
struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition Health_SourceDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealth_SourceDef;
	FGameplayEffectAttributeCaptureDefinition Health_TargetDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealth_TargetDef;
	FGameplayEffectAttributeCaptureDefinition AttackDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition DefensePenetrationDef;
	FGameplayEffectAttributeCaptureDefinition CriticalRateDef;
	FGameplayEffectAttributeCaptureDefinition CriticalDamageDef;
	FGameplayEffectAttributeCaptureDefinition DamageBonusDef;
	FGameplayEffectAttributeCaptureDefinition DamageReductionDef;
	FGameplayEffectAttributeCaptureDefinition FlatDamageBonusDef;
	FGameplayEffectAttributeCaptureDefinition FlatDamageReductionDef;

	FDamageStatics()
	{
		Health_SourceDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		MaxHealth_SourceDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		Health_TargetDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		MaxHealth_TargetDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		AttackDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		DefenseDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		DefensePenetrationDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetDefensePenetrationAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		CriticalRateDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetCriticalRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		CriticalDamageDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetCriticalDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		DamageBonusDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetDamageBonusAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		DamageReductionDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetDamageReductionAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		FlatDamageBonusDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetFlatDamageBonusAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		FlatDamageReductionDef = FGameplayEffectAttributeCaptureDefinition(UGLDAttributeSetCharacter::GetFlatDamageReductionAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
	}
};
//属性捕获 3 实例化,只需要做一次
static FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UGLDGEExecutionCalculationDamage::UGLDGEExecutionCalculationDamage()
{
	//属性捕获 4 类内获取
	RelevantAttributesToCapture.Add(DamageStatics().Health_SourceDef);
	RelevantAttributesToCapture.Add(DamageStatics().MaxHealth_SourceDef);
	RelevantAttributesToCapture.Add(DamageStatics().Health_TargetDef);
	RelevantAttributesToCapture.Add(DamageStatics().MaxHealth_TargetDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefensePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatics().FlatDamageBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics().FlatDamageReductionDef);
}

void UGLDGEExecutionCalculationDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//双方信息初始化
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluateParameters, BaseDamage);
#pragma region DamageCalculate
	// 捕获防御相关属性
	float Defense = 0.f;
	float DefensePenetration = 0.f;
	// 捕获暴击相关属性
	float CriticalRate = 0.f;
	float CriticalDamage = 0.f;
	// 捕获伤害增减乘区
	float DamageBonus = 0.f;
	float DamageReduction = 0.f;
	// 捕获固定伤害和减免
	float FlatDamageBonus = 0.f;
	float FlatDamageReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluateParameters, Defense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePenetrationDef, EvaluateParameters, DefensePenetration);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalRateDef, EvaluateParameters, CriticalRate);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageDef, EvaluateParameters, CriticalDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageBonusDef, EvaluateParameters, DamageBonus);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvaluateParameters, DamageReduction);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FlatDamageBonusDef, EvaluateParameters, FlatDamageBonus);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FlatDamageReductionDef, EvaluateParameters, FlatDamageReduction);

	// 计算防御承伤系数
	float DefenseFactor = FMath::Max(0.f, Defense - DefensePenetration);
	float DefenseCoefficient = 500.f / (500.f + DefenseFactor);
	// 暴击判定
	bool bIsCritical = FMath::FRand() < CriticalRate;
	float CritMultiplier = 1.f + (bIsCritical ? CriticalDamage : 0.f);
	// 增减乘区计算
	float DamageMultiplier =DamageBonus - DamageReduction;
	// 固定伤害区间
	float FixedDamage = FMath::Max(0.f, FlatDamageBonus - FlatDamageReduction);
	// 计算最终伤害
	float FinalDamage = BaseDamage * DefenseCoefficient * CritMultiplier * DamageMultiplier + FixedDamage;
	FinalDamage =  FMath::Max(1.f, FinalDamage);

#pragma endregion

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UGLDAttributeSetCharacter::GetDamageAttribute(), EGameplayModOp::Additive, BaseDamage));
}
