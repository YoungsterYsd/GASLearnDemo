#include "GLDInventoryComponent.h"

#include "Net/UnrealNetwork.h"

#include "GLDPotion.h"
#include "GLDEquipment.h"

#include "AbilitySystemComponent.h"
#include "GLDGameplayAbility.h"
#include "GLDCharacterBase.h"

#include "GLDEquipmentComponent.h"


FGLDInventoryItem::FGLDInventoryItem()
	: RPGItem(nullptr)
	, ItemCount(0)
{





}

bool FGLDInventoryItem::IsValid() const
{
	if (RPGItem && ItemCount > 0)
	{
		return true;

	}
	return false;
}

void FGLDInventoryItem::ResetSelf()
{
	RPGItem = nullptr;
	ItemCount = 0;


}

UGLDInventoryComponent::UGLDInventoryComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InventoryItems.AddDefaulted(30);

	SetIsReplicatedByDefault(true);

}

void UGLDInventoryComponent::OnRep_InventoryItems()
{
	OnInventoryItemChanged.ExecuteIfBound(InventoryItems);
}

void UGLDInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGLDInventoryComponent::ActiveSkillByInventoryId(int32 InInventoryId)
{
	if (InventoryItems[InInventoryId].IsValid())
	{
		if (UGLDPotion* InPotion = Cast<UGLDPotion>(InventoryItems[InInventoryId].RPGItem))
		{
			UAbilitySystemComponent* AbilitySystemComponent = Cast<AGLDCharacterBase>(GetOwner())->GetAbilitySystemComponent();

			UGLDGameplayAbility* InGA = Cast<UGLDGameplayAbility>(InPotion->GrantedAbility->GetDefaultObject());

			FGameplayAbilitySpecHandle  Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(InGA));

			AbilitySystemComponent->TryActivateAbility(Handle);

			//自动移除handle
			AbilitySystemComponent->SetRemoveAbilityOnEnd(Handle);

		}
		else if (UGLDEquipment* InEquipment = Cast<UGLDEquipment>(InventoryItems[InInventoryId].RPGItem))
		{
			UGLDEquipmentComponent* EquipmentComponent = 
			Cast<UGLDEquipmentComponent>(GetOwner()->FindComponentByClass(UGLDEquipmentComponent::StaticClass()));


			//通知服务器装备装备
			FGLDEquipmentItem InEquipmentItem;
			InEquipmentItem.RPGEquimentItemPointer = InEquipment;

			//通知装备组件装备一个装备
			if (EquipmentComponent)
			{
				bool bAddEquipment = EquipmentComponent->AddEquipmentItem(InEquipmentItem);
				if (bAddEquipment)
				{
					RemoveInventoryItem(InInventoryId);

				}

			}


		}
	}
}

void UGLDInventoryComponent::CallServerDownLoadInfo()
{
	InventoryItemChanged(InventoryItems);
}

void UGLDInventoryComponent::InventoryItemChanged_Implementation(const TArray<FGLDInventoryItem>& InInventoryItems)
{
	OnInventoryItemChanged.ExecuteIfBound(InventoryItems);
}

bool UGLDInventoryComponent::AddInventoryItem(FGLDInventoryItem NewItem)
{
	//只在服务器操作数据
	if (HasAuthority())
	{

		if (!NewItem.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
			return false;

		}

		//找一找 原先的仓库有没有
		bool bFind = false;
		for (size_t i = 0; i < InventoryItems.Num(); i++)
		{
			if (InventoryItems[i].RPGItem == NewItem.RPGItem)
			{
				if (NewItem.RPGItem->MaxCount > 1)
				{

					InventoryItems[i].ItemCount += NewItem.ItemCount;
					bFind = true;
					return true;
				}
			}

		}

		//去找一下有米有空的位置
		for (size_t i = 0; i < InventoryItems.Num(); i++)
		{
			if (!InventoryItems[i].IsValid())
			{
				InventoryItems[i] = NewItem;
				return true;
			}
		}


	}
	//	仓库满了
	return false;
}

bool UGLDInventoryComponent::HasThisInventoryItem(FGLDInventoryItem NewItem)
{
	for (size_t i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].RPGItem == NewItem.RPGItem
		&& InventoryItems[i].ItemCount == NewItem.ItemCount)
		{
			return true;
		}
	}



	return false;
}

bool UGLDInventoryComponent::ReplaceInventoryItem(FGLDInventoryItem NewItem, int32 InReplace_ID)
{
	if (HasAuthority())
	{
		InventoryItems[InReplace_ID] = NewItem;

		return true;
	}

	return false;
}

void UGLDInventoryComponent::RemoveInventoryItem(int32 Index_Remove)
{
	if (HasAuthority())
	{

		InventoryItems[Index_Remove].ResetSelf();


	}
}

void UGLDInventoryComponent::RemoveInventoryItemNyItem(FGLDInventoryItem NewItem)
{

	if (HasAuthority())
	{
		for (size_t i = 0; i < InventoryItems.Num(); i++)
		{
			if (InventoryItems[i].RPGItem == NewItem.RPGItem)
			{
				//减去目标数量
				InventoryItems[i].ItemCount -= NewItem.ItemCount;

				if (InventoryItems[i].ItemCount < 0)
				{
					InventoryItems[i].ResetSelf();
				}
				break;
			}

		}
	}
}

void UGLDInventoryComponent::SwapInventoryItem(int32 Index_i, int32 Index_j)
{
	if (HasAuthority())
	{
		InventoryItems.Swap(Index_i, Index_j);
	}
}

UGLDItem* UGLDInventoryComponent::GetInventoryItemByID(int32 i)
{

	if (!InventoryItems[i].IsValid())
	{

		return nullptr;
	}



	return InventoryItems[i].RPGItem;

}

FGLDInventoryItem UGLDInventoryComponent::GetInventoryItemInfoByID(int32 i)
{

	return InventoryItems[i];

}

void UGLDInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGLDInventoryComponent, InventoryItems);
}
