#include "GLDHitBox.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GLDCharacterBase.h"
#include "Abilities/GameplayAbilityTypes.h"




AGLDHitBox::AGLDHitBox(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	HitCollisionRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRootComponent"));
	RootComponent = HitCollisionRootComponent;
	HitDamage = CreateDefaultSubobject<UBoxComponent>((TEXT("HitDamage")));
	HitDamage->SetupAttachment(RootComponent);
	InitialLifeSpan = 4.f;
	bNetLoadOnClient = false;
	bReplicates = false;
}

void AGLDHitBox::HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//首先排除自己  在通知攻击函数里面传入， 是谁生成了它
	if (GetInstigator() != OtherActor)
	{
		if (AGLDCharacterBase* InPawn = Cast<AGLDCharacterBase>(GetInstigator()))
		{
			if (AGLDCharacterBase* InTarget = Cast<AGLDCharacterBase>(OtherActor))
			{
				//必须在服务器
				if (!InPawn->IsNetMode(ENetMode::NM_Client))
				{				
					//传给GAS的事件数据
					FGameplayEventData EventData;
					EventData.Instigator = GetInstigator();
					EventData.Target = InTarget;

					//已经对该对象产生伤害
					if (IsExist(InTarget))
					{
						return;
					}

					if (!BuffsTags.IsEmpty()) //BuffTags
					{
						for (auto& Tmp : BuffsTags)
						{
							//数据上的处理 受伤 //激活GA的命中事件
							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), Tmp.Last(), EventData);
						}
					}
					//标记为已处理
					AttackedTarget.AddUnique(InTarget);
				}
			}
		}
	}
}

UPrimitiveComponent* AGLDHitBox::GetHitDamage()
{
	return HitDamage;
}

void AGLDHitBox::SetHitDamageRelativePosition(const FVector& InNewPostion)
{
	if (UPrimitiveComponent* InHitComponent = GetHitDamage())
	{
		InHitComponent->SetRelativeLocation(InNewPostion);
	}
}

void AGLDHitBox::SetBoxExtent(const FVector& InNewBoxExtent)
{
	HitDamage->SetBoxExtent(InNewBoxExtent);
}

bool AGLDHitBox::IsExist(AGLDCharacterBase* InNewTaget) const
{
	for (auto& Tmp : AttackedTarget)
	{
		if (Tmp.IsValid())
		{
			if (Tmp == InNewTaget)
			{
				return true;
			}
		}
	}
	return false;
}

void AGLDHitBox::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* InHitComponent = GetHitDamage())
	{
		InHitComponent->SetHiddenInGame(true);
		InHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AGLDHitBox::HandleDamage);

	}
}


