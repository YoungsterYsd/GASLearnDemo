// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GLDAnimNotify_ResetCombo.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDAnimNotify_ResetCombo : public UAnimNotify
{
	GENERATED_BODY()
public:
	UGLDAnimNotify_ResetCombo();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
