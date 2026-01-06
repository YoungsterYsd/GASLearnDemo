// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GLDAssetManager.h"
#include "GLDLogChannels.h"
#include "GLDItem.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType UGLDAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType UGLDAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType UGLDAssetManager::EquipmentItemType = TEXT("Equipment");

void UGLDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	//TODo
	//用于初始化AbilitySystemGlobals单例
	UAbilitySystemGlobals::Get().InitGlobalData();
}

UGLDItem* UGLDAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	return nullptr;
}
