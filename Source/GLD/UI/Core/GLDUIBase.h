// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GLDUIBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESlotType : uint8
{
	SlotType_None UMETA(DisplayName = "None_Type"),
	SlotType_SkillPanelSlot UMETA(DisplayName = "SkillPanel_Slot"),
	SlotType_InventorySlot UMETA(DisplayName = "Inventory_Slot"),
	SlotType_EquipmentSlot UMETA(DisplayName = "Equipment_Slot"),
};
class AGLDCharacterBase;
class UWidgetAnimation;
UCLASS()
class GLD_API UGLDUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UGLDUIBase(const FObjectInitializer& ObjInitor);

	void PlayWidgetAnim(const FString& InAnimName);
	
	UWidgetAnimation* GetNameWidgetAnimation(const  FString& InWigetName);

	AGLDCharacterBase* UI_GetGLDCharacterBase();
	
};
