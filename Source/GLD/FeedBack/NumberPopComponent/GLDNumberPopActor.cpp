#include "GLDNumberPopActor.h"
#include "Components/WidgetComponent.h"
#include "UI_DamageNum.h"


AGLDDamageNumActor::AGLDDamageNumActor()
{
	DamageNumRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DamageNumRootComponent"));
	RootComponent = DamageNumRootComponent;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DamgeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	DamgeWidgetComponent->SetupAttachment(RootComponent);
	DamgeWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InitialLifeSpan = 4.0;


}

void AGLDDamageNumActor::UpdateNum(const FText& InText)
{
	if (DamgeWidgetComponent)
	{
		if (UUI_DamageNumber* Damge = Cast<UUI_DamageNumber>(DamgeWidgetComponent->GetUserWidgetObject()))
		{
			Damge->UpdateNum(InText);
		}
	}

}

void AGLDDamageNumActor::UpdateNumColor(const FLinearColor& InColor)
{
	if (DamgeWidgetComponent)
	{

		if (UUI_DamageNumber* Damge = Cast<UUI_DamageNumber>(DamgeWidgetComponent->GetUserWidgetObject()))
		{
			Damge->UpdateNumColor(InColor);
		}


	}
}
