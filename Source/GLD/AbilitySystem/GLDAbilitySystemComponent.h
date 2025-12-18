// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GLDAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UGLDAbilitySystemComponent(const FObjectInitializer& objInitor = FObjectInitializer::Get());
	
};
