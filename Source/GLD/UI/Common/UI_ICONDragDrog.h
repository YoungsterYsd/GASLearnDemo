// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GLDUIBase.h"
#include "UI_ICONDragDrog.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class GLD_API UUI_ICONDragDrog : public UGLDUIBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* ICON;

public:
	void DrawICON(UTexture2D* InICON);


};
