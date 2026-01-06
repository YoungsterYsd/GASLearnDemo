#pragma once

#include "CoreMinimal.h"
#include "GLDItem.h"

#include "Components/GameFrameworkComponent.h"
#include "GLDComponentBase.h"

#include "GLDInventoryComponent.generated.h"



class AGLDCharacterBase;
class UGLDAbilitySystemComponent;
class UGLDItem;



USTRUCT(BlueprintType)
struct  FGLDInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:
	FGLDInventoryItem();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RPGInventoryItem)
	UGLDItem* RPGItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RPGInventoryItem)
	int32 ItemCount;

	bool IsValid() const;

	void ResetSelf();

};

DECLARE_DELEGATE_OneParam(FOnInventoryItemChangedNative, const TArray<FGLDInventoryItem>&);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GLD_API UGLDInventoryComponent : public UGLDComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGLDInventoryComponent(const FObjectInitializer& ObjectInitializer);

	//给UI绑定的代理
	//仓库的代理
	FOnInventoryItemChangedNative OnInventoryItemChanged;

	UFUNCTION()
	void OnRep_InventoryItems();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	void ActiveSkillByInventoryId(int32  InInventoryId);

	void CallServerDownLoadInfo();

	//仅供内部使用
	//通知UI刷新界面
	//为什么会有参数 因为是RPC需要同步,如果在方法体内执行内,拖取的数据是客户端的
	UFUNCTION(Client, Reliable)
	void InventoryItemChanged(const TArray<FGLDInventoryItem>& InInventoryItems);


	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(FGLDInventoryItem NewItem);

	UFUNCTION(BlueprintPure, Category = Inventory)
	bool HasThisInventoryItem(FGLDInventoryItem NewItem);

	//添加到制定位置
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool ReplaceInventoryItem(FGLDInventoryItem NewItem, int32 InReplace_ID);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveInventoryItem(int32 Index_Remove);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveInventoryItemNyItem(FGLDInventoryItem NewItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void SwapInventoryItem(int32 Index_i, int32 Index_j);

	//获取制定仓库指定栏位的物品
	UFUNCTION(BlueprintCallable, Category = Inventory)
	UGLDItem* GetInventoryItemByID(int32 i);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FGLDInventoryItem GetInventoryItemInfoByID(int32 i);





protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_InventoryItems, Category = Inventory)
	TArray<FGLDInventoryItem> InventoryItems;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



};

