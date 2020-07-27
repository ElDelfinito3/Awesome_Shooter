#include "DynamicLight.h"
#include "UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

ADynamicLight::ADynamicLight()
{
	//set default values
	{
		LightColor = White;
		IsLightActive = true;
	}

	//set point light as root
	{
		PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
		PointLight->Intensity = 10000.f;
		PointLight->bVisible = true;
		RootComponent = PointLight;
	}

	//set switch visibility trigger
	{
		SwitchVisibilityTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Switch Visibility Trigger"));
		SwitchVisibilityTrigger->SetupAttachment(RootComponent);
		SwitchVisibilityTrigger->InitBoxExtent(FVector(50.0f, 50.0f, 30.0f));
		SwitchVisibilityTrigger->SetWorldLocation(FVector(-410.f, -310.f, -250.f));
		SwitchVisibilityTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicLight::SwitchLightVisibility);
	}

	//set yellow trigger
	{
		YellowTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Yellow Trigger Box"));
		YellowTrigger->SetupAttachment(RootComponent);
		YellowTrigger->InitBoxExtent(FVector(100.0f, 100.0f, 30.0f));
		YellowTrigger->SetWorldLocation(FVector(-250.f, 10.f, -250.f));
		YellowTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicLight::SetLightToYellow);
	}

	//set blue trigger
	{
		BlueTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Blue Trigger Box"));
		BlueTrigger->SetupAttachment(RootComponent);
		BlueTrigger->InitBoxExtent(FVector(100.0f, 100.0f, 30.0f));
		BlueTrigger->SetWorldLocation(FVector(-480.f, 10.f, -250.f));
		BlueTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicLight::SetLightToBlue);
	}
	
	//set red trigger
	{
		RedTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Red Trigger Box"));
		RedTrigger->SetupAttachment(RootComponent);
		RedTrigger->InitBoxExtent(FVector(100.0f, 100.0f, 30.0f));
		RedTrigger->SetWorldLocation(FVector(-480.f, 240.f, -250.f));
		RedTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicLight::SetLightToRed);
	}

	//set green trigger
	{
		GreenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Green Trigger Box"));
		GreenTrigger->SetupAttachment(RootComponent);
		GreenTrigger->InitBoxExtent(FVector(100.0f, 100.0f, 30.0f));
		GreenTrigger->SetWorldLocation(FVector(-250.f, 240.f, -250.f));
		GreenTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicLight::SetLightToGreen);
	}

	SetReplicates(true);
}

void ADynamicLight::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{OnRep_InitLightVisibility(); OnRep_InitLightColor();}
}

void ADynamicLight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADynamicLight, IsLightActive);
	DOREPLIFETIME(ADynamicLight, LightColor);
}

void ADynamicLight::OnRep_InitLightVisibility()
{
	if (IsLightActive) {PointLight->SetVisibility(true);}
	else {PointLight->SetVisibility(false);}
}

void ADynamicLight::OnRep_InitLightColor()
{
	switch (LightColor)
	{
		case White: PointLight->SetLightColor(FLinearColor(1.f, 1.f, 1.f)); break;
		case Yellow: PointLight->SetLightColor(FLinearColor(0.625f, 0.60326f, 0.f)); break;
		case Blue: PointLight->SetLightColor(FLinearColor(0.f, 0.f, 1.f)); break;
		case Red: PointLight->SetLightColor(FLinearColor(1.f, 0.f, 0.f)); break;
		case Green: PointLight->SetLightColor(FLinearColor(0.f, 1.f, 0.f)); break;
	}
}

void ADynamicLight::SwitchLightVisibility(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{
		if (IsLightActive) {IsLightActive = false; OnRep_InitLightVisibility();}
		else {IsLightActive = true; OnRep_InitLightVisibility();}
	}
}

void ADynamicLight::SetLightToYellow(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{LightColor = Yellow; OnRep_InitLightColor();}
}

void ADynamicLight::SetLightToBlue(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{LightColor = Blue; OnRep_InitLightColor();}
}

void ADynamicLight::SetLightToRed(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{LightColor = Red; OnRep_InitLightColor();}
}

void ADynamicLight::SetLightToGreen(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{LightColor = Green; OnRep_InitLightColor();}
}


