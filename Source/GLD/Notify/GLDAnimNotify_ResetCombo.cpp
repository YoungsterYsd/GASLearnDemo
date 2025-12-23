// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/GLDAnimNotify_ResetCombo.h"
#include "GLDCharacterBase.h"
#include "GLDComboComponent.h"

UGLDAnimNotify_ResetCombo::UGLDAnimNotify_ResetCombo()
{
}

FString UGLDAnimNotify_ResetCombo::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UGLDAnimNotify_ResetCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if(AGLDCharacterBase* Character = Cast<AGLDCharacterBase>(MeshComp->GetOwner()))
	{

		Character->GetGLDComboComponent()->ResetComboIndex();
	}
}
