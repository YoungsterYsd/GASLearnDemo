// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GLDGEExecutionCalculationDamage.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDGEExecutionCalculationDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGLDGEExecutionCalculationDamage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	// 多个攻击方属性捕获及其加成倍率，需根据项目扩展
	static TArray<FGameplayEffectAttributeCaptureDefinition> AttackAttributes;
	static TArray<float> AttackAttributeMultipliers;
};
