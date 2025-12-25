#pragma once

#include "Components/ControllerComponent.h"
#include "GameplayTagContainer.h"
#include "GLDNumberPopComponent.h"
#include "GLDNumberPopActor.h"
#include "GLDNumberPopComponent_UMG.generated.h"

class AGLDDamageNumActor;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class UGLDNumberPopComponent_UMG : public UGLDNumberPopComponent
{
	GENERATED_BODY()

public:

	UGLDNumberPopComponent_UMG(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Adds a damage number to the damage number list for visualization */
	virtual void AddNumberPop(const FGLDNumberPopRequest& NewRequest);

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GLD|DamgeNum", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGLDDamageNumActor> NumPopActorClass;



};