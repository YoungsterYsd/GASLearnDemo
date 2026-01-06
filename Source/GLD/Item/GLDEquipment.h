#pragma once

#include "CoreMinimal.h"
#include "GLDItem.h"

#include "GLDEquipment.generated.h"

class UGameplayEffect;
/**
 *
 */
UCLASS()
class GLD_API UGLDEquipment : public UGLDItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	UGLDEquipment();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

};
