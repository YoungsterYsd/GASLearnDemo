#pragma once

#include "CoreMinimal.h"
#include "GLDUIBase.h"
#include "UI_DamageNum.generated.h"

/**
 *
 */
class UTextBlock;

UCLASS()
class GLD_API UUI_DamageNumber : public UGLDUIBase
{

	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumTex;

public:
	virtual void NativeConstruct();

	void	UpdateNum(const FText& InText);
	void	UpdateNumColor(const FLinearColor& InColor);
};