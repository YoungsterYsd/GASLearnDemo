// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GLDComboComponent.h"

UGLDComboComponent::UGLDComboComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UGLDComboComponent::GetComboIndex()
{
	return ComboIndex;
}

UAnimMontage* UGLDComboComponent::GetComboAnimMontage()
{
	CurrentMontage = ComboMontages.IsValidIndex(ComboIndex) ? ComboMontages[ComboIndex] : ComboMontages[0];

	return CurrentMontage;
}

UAnimMontage* UGLDComboComponent::GetLastPlayAnimMontage()
{
	return CurrentMontage;
}

void UGLDComboComponent::UpdateComboIndex()
{
	ComboIndex++;
	if(ComboIndex > ComboMaxIndex)
	{
		ComboIndex = 0;
	}
}

void UGLDComboComponent::ResetComboIndex()
{
	ComboIndex = 0;
}

void UGLDComboComponent::SetPressed()
{
	bIsComboInputOn = true;
}

void UGLDComboComponent::ResetPressed()
{
	bIsComboInputOn = false;
}

bool UGLDComboComponent::IsStillPressed()
{
	return bIsComboInputOn;
}


void UGLDComboComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ComboMontages.Num() > 0)
	{
		ComboMaxIndex = ComboMontages.Num() - 1;
	}
}
