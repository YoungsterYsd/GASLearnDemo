// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameplayAbilitySpecHandle.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayCueInterface.h"
#include "GLDPlayerController.h"
#include "GLDCharacterBase.generated.h"

class UGLDAbilitySystemComponent;
class UGLDComboComponent;
class AGLDPlayerController;
class AGLDPlayerState;
class UGLDGameplayAbility;
class UGLDAttributeSetCharacter;
class UGLDHealthComponent;
class UGLDInventoryComponent;
class UGLDEquipmentComponent;

//做冷却广播
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGenericAbilityCoolDownDelegate, FGameplayTagContainer, OutAbilityTag, float, CooldownTime);

UCLASS()
class GLD_API AGLDCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,public IGameplayCueInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGLDCharacterBase(const FObjectInitializer& objInitor = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//各类功能组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDComboComponent> ComboComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDHealthComponent> HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDInventoryComponent> InventoryComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDEquipmentComponent> EquipmentComp;

	//能力处理
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GLD|Character",meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TSubclassOf<UGLDGameplayAbility>> AbilitiesToAdd;
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> AbilitiesToActive;
	//DeathHandle
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GLD|Character", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGLDGameplayAbility> DeathAbilityClass;
	FGameplayAbilitySpecHandle DeathAbilityHandle;

	TObjectPtr<UGLDAttributeSetCharacter> AttributeSet;
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientRPCFunction(FGameplayTagContainer OutAbilityTag, float CooldownTime);
public:
	//蓝图可调用的组件获取接口
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	UGLDAbilitySystemComponent* GetGLDAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	AGLDPlayerController* GetGLDPlayerController() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	AGLDPlayerState* GetGLDPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	UGLDComboComponent* GetGLDComboComponent() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	UGLDHealthComponent* GetGLDHealthComponent() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	UGLDInventoryComponent* GetGLDInventoryComponent() const;
	UFUNCTION(BlueprintCallable, Category = "GLD|Character")
	UGLDEquipmentComponent* GetGLDEquipmentComponent() const;

	UFUNCTION(Server, Reliable)
	void ActiveSkillByInventoryId(int32 InInventoryID);
	UFUNCTION(Server, Reliable)
	void CallServerDownLoadInfo();

	//处理道具交换
	UFUNCTION(Server, Reliable)
	void SwapInventoryItem(int32 Index_i,int32 Index_j);
	UFUNCTION(Server, Reliable)
	void SwapEquipmentItem(int32 Index_i, int32 Index_j);

	//处理装备解锁
	UFUNCTION(Server, Reliable)
	void UndockEquipmentByID(int32 InUndockEquipmentID);
	
	//处理装备穿戴
	UFUNCTION(Server, Reliable)
	void SwapFromInvToEqu(int32 InInventory_Index, int32 InEquipment_Index);
	UFUNCTION(Server, Reliable)
	void SwapFromEquToInv(int32 InEquipment_Index, int32 InInventory_Index);

	UPROPERTY(BlueprintAssignable)
	FGenericAbilityCoolDownDelegate AbilityCoolDownDelegate;

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

};
