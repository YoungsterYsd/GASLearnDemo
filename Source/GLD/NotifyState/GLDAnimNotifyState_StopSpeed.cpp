// Fill out your copyright notice in the Description page of Project Settings.


#include "GLDAnimNotifyState_StopSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGLDAnimNotifyState_StopSpeed::UGLDAnimNotifyState_StopSpeed()
	:MaxWalkSpeed(0.f)
{

}

void UGLDAnimNotifyState_StopSpeed::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{
		//存储一份
		MaxWalkSpeed = InCharacter->GetCharacterMovement()->MaxWalkSpeed;
		MinAnalogWalkSpeed = InCharacter->GetCharacterMovement()->MinAnalogWalkSpeed;
		InCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.f;
		InCharacter->GetCharacterMovement()->MinAnalogWalkSpeed = 0.f;
	}

}

void UGLDAnimNotifyState_StopSpeed::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGLDAnimNotifyState_StopSpeed::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{
		InCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		InCharacter->GetCharacterMovement()->MinAnalogWalkSpeed = MinAnalogWalkSpeed;
	}
}
