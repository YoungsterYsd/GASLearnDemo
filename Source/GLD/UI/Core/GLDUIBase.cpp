// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/GLDUIBase.h"
#include "GLDCharacterBase.h"

#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Animation/WidgetAnimation.h"

UGLDUIBase::UGLDUIBase(const FObjectInitializer& ObjInitor):Super(ObjInitor)
{
}

void UGLDUIBase::PlayWidgetAnim(const FString& InAnimName)
{
	if (UWidgetAnimation* TempAnimation = GetNameWidgetAnimation(InAnimName))
	{
		PlayAnimation(TempAnimation);
	}
}

UWidgetAnimation* UGLDUIBase::GetNameWidgetAnimation(const FString& InWigetName)
{
	
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		//获取动画
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;

		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation) {
				return  OurAnimation->GetFName().ToString() == (InWigetName + FString("_INST"));
			});
		return  *MyTempAnimation;
	}
	return nullptr;
}

AGLDCharacterBase* UGLDUIBase::UI_GetGLDCharacterBase()
{
	//从世界获取当前控制角色
	if (GetWorld())
	{
		if (APlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<APlayerController>())
		{
			if (AGLDCharacterBase* InCharacter = InPlayerController->GetPawn<AGLDCharacterBase>())
			{
				return	 InCharacter;
			}
		}
	}

	return nullptr;
}
