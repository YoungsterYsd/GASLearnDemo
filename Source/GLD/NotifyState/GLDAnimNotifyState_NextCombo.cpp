// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState/GLDAnimNotifyState_NextCombo.h"
#include "GLDCharacter.h"
#include "GLDComboComponent.h"

UGLDAnimNotifyState_NextCombo::UGLDAnimNotifyState_NextCombo()
{
}

void UGLDAnimNotifyState_NextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	//确认不是服务器端
	if(MeshComp->GetOuter()&& MeshComp->GetOuter()->GetWorld()&& !MeshComp->GetOuter()->GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		if(AGLDCharacterBase* InCharacter = Cast<AGLDCharacterBase>(MeshComp->GetOuter()))
		{
			InCharacter->GetGLDComboComponent()->ResetPressed();
		}
	}

}

void UGLDAnimNotifyState_NextCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);


}

void UGLDAnimNotifyState_NextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp->GetOuter() && MeshComp->GetOuter()->GetWorld() && !MeshComp->GetOuter()->GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		if(AGLDCharacter* InCharacter = Cast<AGLDCharacter>(MeshComp->GetOuter()))
		{
			if (InCharacter->GetGLDComboComponent()->IsStillPressed())
			{
				InCharacter->ComboMeleeAttack();
			}
		}
	}
}
