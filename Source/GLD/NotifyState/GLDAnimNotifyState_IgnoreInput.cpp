// Fill out your copyright notice in the Description page of Project Settings.


#include "GLDAnimNotifyState_IgnoreInput.h"
#include "GameFramework/Character.h"
UGLDAnimNotifyState_IgnoreInput::UGLDAnimNotifyState_IgnoreInput()
{

}

void UGLDAnimNotifyState_IgnoreInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{

		//网络模式不是客户端
		if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Client))
		{
			//是不是主机代理，就是客户端在自己的客户端
			//客户端在别人客户端就是模拟玩家
			if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
			{
				InCharacter->DisableInput(InCharacter->GetWorld()->GetFirstPlayerController());
			}
		}
		else if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Standalone) ||
			InCharacter->GetWorld()->IsNetMode(ENetMode::NM_ListenServer))
		{
			InCharacter->DisableInput(InCharacter->GetWorld()->GetFirstPlayerController());
		}
	}

}

void UGLDAnimNotifyState_IgnoreInput::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}


void UGLDAnimNotifyState_IgnoreInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
	{
		if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Client))
		{

			if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
			{
				InCharacter->EnableInput(InCharacter->GetWorld()->GetFirstPlayerController());
			}

		}
		else if (InCharacter->GetWorld()->IsNetMode(ENetMode::NM_Standalone)
			|| InCharacter->GetWorld()->IsNetMode(ENetMode::NM_ListenServer))
		{
			InCharacter->EnableInput(InCharacter->GetWorld()->GetFirstPlayerController());
		}
	}

}
