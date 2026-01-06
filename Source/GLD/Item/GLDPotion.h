#pragma once


#include "CoreMinimal.h"

#include "GLDItem.h"
#include "GLDPotion.generated.h"


class UGLDGameplayAbility;

UCLASS()
class GLD_API UGLDPotion : public UGLDItem
{
	GENERATED_BODY()
public:
	/** Constructor */
	UGLDPotion();

public:
	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UGLDGameplayAbility> GrantedAbility;

	/** Ability level this item grants. <= 0 means the character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 AbilityLevel;



};