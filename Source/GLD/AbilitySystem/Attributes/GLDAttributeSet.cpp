// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GLDAttributeSet.h"
#include "AbilitySystem/GLDAbilitySystemComponent.h"

UGLDAttributeSet::UGLDAttributeSet()
{
}

UWorld* UGLDAttributeSet::GetWorld() const
{
	return GetOuter()->GetWorld();
}

UGLDAbilitySystemComponent* UGLDAttributeSet::GetGLDAbilitySystemComponent() const
{
	return Cast<UGLDAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

