// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/GLDActorComponentBase.h"
#include "GLDComboComponent.generated.h"

/**
 * 
 */
UCLASS()
class GLD_API UGLDComboComponent : public UGLDActorComponentBase
{
	GENERATED_BODY()
public:
	UGLDComboComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Combo")
	int32 GetComboIndex();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	UAnimMontage* GetComboAnimMontage();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	UAnimMontage* GetLastPlayAnimMontage();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void UpdateComboIndex();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetComboIndex();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void SetPressed();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetPressed();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	bool IsStillPressed();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	TArray<UAnimMontage*> ComboMontages;

	UPROPERTY()
	UAnimMontage* CurrentMontage = nullptr;

	int32 ComboIndex = 0;
	int32 ComboMaxIndex = 0;
	bool bIsComboInputOn = false;

};
