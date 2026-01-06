#pragma once

#include "CoreMinimal.h"
#include "GLDUIBase.h"

#include "UI_SlotBase.generated.h"

class UImage;
class UButton;

/**
 *
 */
UCLASS()
class GLD_API UUI_SlotBase : public UGLDUIBase
{
	GENERATED_BODY()

public:

	UUI_SlotBase(const FObjectInitializer& ObjectInitializer);

	//用get set方法
	//图片本体
	UPROPERTY(meta = (BindWidget))
	UImage* SlotIcon;

	UPROPERTY(meta = (BindWidget))
	UButton* ClickButton;

	//配置蓝图类
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUI_InformationDescribe> UI_TipClass;

public:

	virtual void NativeConstruct() override;

	void HideSlotIcon();
	void ShowSlotIcon();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Tip)
	UWidget* GetSlotTip();

	UFUNCTION()
	virtual void OnClickedWidget() {};

public:

	FORCEINLINE ESlotType GetSlotType() {
		return SlotType;
	};

protected:
	ESlotType SlotType;
};