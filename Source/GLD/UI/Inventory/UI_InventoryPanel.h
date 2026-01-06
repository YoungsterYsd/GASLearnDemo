#pragma once

#include "CoreMinimal.h"
#include "GLDUIBase.h"
#include "Components/UniformGridPanel.h"
#include "GLDInventoryComponent.h"
#include "UI_InventoryPanel.generated.h"

class UUI_InventorySlot;
UCLASS()
class GLD_API UUI_InventoryPanel : public UGLDUIBase
{
	GENERATED_BODY()
	//框框 用来装元素 类似于水平框 竖直框
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryArray;

	//蓝图配置
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_InventorySlot> InventoryClass;

public:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//用来接受仓库组件发来的 信息
	UFUNCTION()
	void OnInventoryItemChanged(const TArray<FGLDInventoryItem>& InInventoryItems);


};