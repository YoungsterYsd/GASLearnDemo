// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GLDAnimNotifyState_StopSpeed.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "StopSpeed"))
class GLD_API UGLDAnimNotifyState_StopSpeed : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UGLDAnimNotifyState_StopSpeed();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	float MaxWalkSpeed;
	float MinAnalogWalkSpeed;
};
