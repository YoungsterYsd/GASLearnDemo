// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/GLDUIBase.h"
#include "GLDUI_CharacterInfo.generated.h"
class UTextBlock;
/**
 * 
 */
UCLASS()
class GLD_API UGLDUI_CharacterInfo : public UGLDUIBase
{
	GENERATED_BODY()
protected:
	//重写构造、析构、tick，必须调用super
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    //本地用的文本拼接函数
    void SetAttr(UTextBlock* InTextBlock, float InNumUp, float InNumDown);
    //本地用的文本拼接函数
    void SetAttr(UTextBlock* InTextBlock, float InNumUp);
    //本地用的文本拼接函数
    void SetAttr_Percentage(UTextBlock* InTextBlock, float InNumUp);
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Level;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Experience;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Health;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Energy;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Attack;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Defense;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DefensePenetration;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CriticalRate;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CriticalDamage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DamageBonus;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DamageReduction;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Sustain;
};
