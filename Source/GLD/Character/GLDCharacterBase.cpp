// Fill out your copyright notice in the Description page of Project Settings.


#include "GLDCharacterBase.h"
#include "GLDPlayerController.h"
#include "GLDPlayerState.h"
#include "GLDGameplayAbility.h"
#include "GLDComboComponent.h"
#include "GLDAttributeSetCharacter.h"
#include "GLDHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/GLDAbilitySystemComponent.h"

// Sets default values
AGLDCharacterBase::AGLDCharacterBase(const FObjectInitializer& objInitor) :Super(objInitor)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;


	AbilitySystemComp = CreateDefaultSubobject<UGLDAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ComboComp = CreateDefaultSubobject<UGLDComboComponent>(TEXT("ComboComp"));
	ComboComp->SetIsReplicated(false);
	AttributeSet = CreateDefaultSubobject<UGLDAttributeSetCharacter>(TEXT("CharacterAttributeSet"));

	HealthComp = CreateDefaultSubobject<UGLDHealthComponent>(TEXT("CharacterHealthComponent"));
	HealthComp->SetIsReplicated(true);
	HealthComp->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComp->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	SetNetUpdateFrequency(100.f);
}

// Called when the game starts or when spawned
void AGLDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystemComp && GetLocalRole() == ENetRole::ROLE_Authority)
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}

	//临时注册能力 AbilitiesToAdd;
	for (const auto& AbilityPair : AbilitiesToAdd)
	{
		if (UGLDGameplayAbility* AbilityCDO = AbilityPair.Value->GetDefaultObject<UGLDGameplayAbility>())
		{
			FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
			AbilitySpec.SourceObject = this;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityPair.Key);
			//向gas系统注册技能拿到handle
			const FGameplayAbilitySpecHandle AbilitySpecHandle =  AbilitySystemComp->GiveAbility(AbilitySpec);
			AbilitiesToActive.Add(AbilityPair.Key, AbilitySpecHandle);
		}
		//死亡能力初始化，获取类、创建Spec、指定来源、给予能力。
		UGLDGameplayAbility* DeathAbilityCDO = DeathAbilityClass->GetDefaultObject<UGLDGameplayAbility>();
		FGameplayAbilitySpec DeathAbilityCDOSpec(DeathAbilityCDO, 1);
		DeathAbilityCDOSpec.SourceObject = this;
		DeathAbilityHandle = AbilitySystemComp->GiveAbility(DeathAbilityCDOSpec);
		//初始化冷却
		AbilitySystemComp->AbilityCommittedCallbacks.AddLambda([this](UGameplayAbility* InGameplayAbility) {

			float CoolDownTime = InGameplayAbility->GetCooldownTimeRemaining();

			FGameplayTagContainer  AbilityTags = InGameplayAbility->AbilityTags;

			ClientRPCFunction(AbilityTags, CoolDownTime);

			});
	}
	HealthComp->InitializeWithAbilitySystem(AbilitySystemComp);
}

void AGLDCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//析构时取消能力注册，其实没必要
	if (AbilitySystemComp && GetLocalRole() == ::ENetRole::ROLE_Authority)
	{
		AbilitySystemComp->ClearAllAbilities();
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGLDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGLDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGLDCharacterBase::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void AGLDCharacterBase::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void AGLDCharacterBase::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	MoveComp->StopMovementImmediately();
	MoveComp->DisableMovement();
}

void AGLDCharacterBase::DestroyDueToDeath()
{
	K2_OnDeathFinished();//调用蓝图表现

	UninitAndDestroy();
}

void AGLDCharacterBase::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}
	HealthComp->UninitializeFromAbilitySystem();
	SetActorHiddenInGame(true);
}


void AGLDCharacterBase::ClientRPCFunction_Implementation(FGameplayTagContainer OutAbilityTag, float CooldownTime)
{   //广播能力冷却
	AbilityCoolDownDelegate.Broadcast(OutAbilityTag, CooldownTime);
}

UGLDAbilitySystemComponent* AGLDCharacterBase::GetGLDAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

UAbilitySystemComponent* AGLDCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp; 
}

AGLDPlayerController* AGLDCharacterBase::GetGLDPlayerController() const
{
	return CastChecked<AGLDPlayerController>(Controller,ECastCheckedType::NullAllowed); 
}

AGLDPlayerState* AGLDCharacterBase::GetGLDPlayerState() const
{
	return CastChecked<AGLDPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UGLDComboComponent* AGLDCharacterBase::GetGLDComboComponent() const
{
	return ComboComp;
}

UGLDHealthComponent* AGLDCharacterBase::GetGLDHealthComponent() const
{
	return HealthComp;
}

void AGLDCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if(const UGLDAbilitySystemComponent* ASC = GetGLDAbilitySystemComponent())
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool AGLDCharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if(const UGLDAbilitySystemComponent* ASC = GetGLDAbilitySystemComponent())
	{
		return ASC->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}

bool AGLDCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(const UGLDAbilitySystemComponent* ASC = GetGLDAbilitySystemComponent())
	{
		return ASC->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool AGLDCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(const UGLDAbilitySystemComponent* ASC = GetGLDAbilitySystemComponent())
	{
		return ASC->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}

