// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatacterInfo/GLDUI_CharacterInfo.h"

#include "GLDAbilitySystemComponent.h"
#include "GLDCharacterBase.h"
#include "GLDAttributeSetCharacter.h"
#include "Components\TextBlock.h"

void UGLDUI_CharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGLDUI_CharacterInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGLDUI_CharacterInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AGLDCharacterBase* InCharacter = UI_GetGLDCharacterBase())
	{
		if (UGLDAbilitySystemComponent* InASC = InCharacter->GetGLDAbilitySystemComponent())
		{
			if (const UGLDAttributeSetCharacter* InAS =
				Cast<UGLDAttributeSetCharacter>(InASC->GetAttributeSet(UGLDAttributeSetCharacter::StaticClass())))
			{
				SetAttr(Level, InAS->GetLevel());
				SetAttr(Experience, InAS->GetExperience(), InAS->GetMaxExperience());
				SetAttr(Health, InAS->GetHealth(), InAS->GetMaxHealth());
				SetAttr(Energy, InAS->GetEnergy(), InAS->GetMaxEnergy());
				SetAttr(Attack, InAS->GetAttack());
				SetAttr(Defense, InAS->GetDefense());
				SetAttr(DefensePenetration, InAS->GetDefensePenetration());
				SetAttr_Percentage(CriticalRate, InAS->GetCriticalRate());
				SetAttr_Percentage(CriticalDamage, InAS->GetCriticalDamage());
				SetAttr_Percentage(DamageBonus, InAS->GetDamageBonus());
				SetAttr_Percentage(DamageReduction, InAS->GetDamageReduction());
				SetAttr_Percentage(Sustain, InAS->GetSustain());
			}
		}
	}
}
void UGLDUI_CharacterInfo::SetAttr(UTextBlock* InTextBlock, float InNumUp, float InNumDown)
{
	if (!InTextBlock)return;
	FString UpString = FString::Printf(TEXT("%s"), *FString::SanitizeFloat(InNumUp));
	FString DownString = FString::Printf(TEXT("%s"), *FString::SanitizeFloat(InNumDown));
	InTextBlock->SetText(FText::FromString(UpString + "/" + DownString));
}

void UGLDUI_CharacterInfo::SetAttr(UTextBlock* InTextBlock, float InNumUp)
{
	if (!InTextBlock)return;
	FString String = FString::Printf(TEXT("%s"), *FString::SanitizeFloat(InNumUp));
	InTextBlock->SetText(FText::FromString(String));
}

void UGLDUI_CharacterInfo::SetAttr_Percentage(UTextBlock* InTextBlock, float InNumUp)
{
	if (!InTextBlock)return;
	FString String = FString::Printf(TEXT("%s"), *FString::SanitizeFloat(InNumUp*100));
	InTextBlock->SetText(FText::FromString(String + "%"));
}
