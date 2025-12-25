#pragma once
#include "UI_DamageNum.h"
#include "Components\TextBlock.h"

void UUI_DamageNumber::NativeConstruct()
{
	PlayWidgetAnim(TEXT("FadeAnimation"));
}


void UUI_DamageNumber::UpdateNum(const FText& InText)
{
	if (NumTex)
	{
		NumTex->SetText(InText);
	}



}


void UUI_DamageNumber::UpdateNumColor(const FLinearColor& InColor)
{
	if (NumTex)
	{
		NumTex->SetColorAndOpacity(FSlateColor(InColor));
	}
}

