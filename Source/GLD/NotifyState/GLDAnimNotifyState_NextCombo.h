// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GLDAnimNotifyState_NextCombo.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDAnimNotifyState_NextCombo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UGLDAnimNotifyState_NextCombo();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	
	
};
