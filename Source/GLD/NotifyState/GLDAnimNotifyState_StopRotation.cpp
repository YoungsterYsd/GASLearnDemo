// Fill out your copyright notice in the Description page of Project Settings.


#include "GLDAnimNotifyState_StopRotation.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGLDAnimNotifyState_StopRotation::UGLDAnimNotifyState_StopRotation()
{

}

void UGLDAnimNotifyState_StopRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{
		InCharacter->bUseControllerRotationYaw = false;
		InCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UGLDAnimNotifyState_StopRotation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGLDAnimNotifyState_StopRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{
		InCharacter->bUseControllerRotationYaw = false;
		InCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}
