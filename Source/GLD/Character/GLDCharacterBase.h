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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDComboComponent> ComboComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GLD|Character")
	TObjectPtr<UGLDHealthComponent> HealthComp;

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

public:
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


public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

};
