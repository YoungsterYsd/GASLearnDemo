// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GLDAssetManager.h"
#include "AbilitySystemGlobals.h"

void UGLDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	//TODo
	//用于初始化AbilitySystemGlobals单例
	UAbilitySystemGlobals::Get().InitGlobalData();
}
