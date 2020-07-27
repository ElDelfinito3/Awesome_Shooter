#include "Destroypack.h"
#include "UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"

ADestroypack::ADestroypack()
{
	//Set Default Values
	{
		FullHealth = 100.f;
		DefaultDamage = 10.f;
		DefaultImpulse = 3000.f;
		IsDestroyed = false;
	}

	//Set Root
	{
		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		RootComponent = Root;
	}

	//Set Collision Sphere
	{
		CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
		CollisionSphere->SetupAttachment(RootComponent);
		CollisionSphere->InitSphereRadius(60.f);
		CollisionSphere->SetCollisionProfileName(TEXT("PhysicsActor"));
		CollisionSphere->OnComponentHit.AddDynamic(this, &ADestroypack::OnHit);
	}

	//Set Destructible Component
	{
		DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
		DestructibleComponent->SetupAttachment(RootComponent);
		DestructibleComponent->SetNotifyRigidBodyCollision(true);
		DestructibleComponent->CastShadow = false;
		//? DestructibleComponent->SetIsReplicated(true);
	}

	//Set Health as Render Text
	{
		TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
		TextRender->SetupAttachment(RootComponent);
		TextRender->SetHorizontalAlignment(EHTA_Center);
		TextRender->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		TextRender->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
		TextRender->SetTextRenderColor(FColor::Red);
	}

	//Set Blood FloorDecal
	{
		BloodDecal = CreateDefaultSubobject<UDecalComponent>("Floor Decal");
		BloodDecal->DecalSize = FVector(150.f, 150.f, 150.f);
		BloodDecal->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
		BloodDecal->SetWorldLocation(FVector(0.f, 0.f, 0.f));
		BloodDecal->SetHiddenInGame(true);
	}

	//Set Blood WallDecal
	//{
	//	WallDecal = CreateDefaultSubobject<UDecalComponent>("Wall Decal");
	//	WallDecal->DecalSize = FVector(30.f, 150.f, 150.f);
	//	WallDecal->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	//	WallDecal->SetHiddenInGame(true);
	//}

	SetReplicates(true);
}

void ADestroypack::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		Health = FullHealth;
		OnRep_UpdateHealth();
		if (IsDestroyed) {OnRep_IsDestroyed();}
	}
}

void ADestroypack::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADestroypack, Health);
	DOREPLIFETIME(ADestroypack, IsDestroyed);
	DOREPLIFETIME(ADestroypack, DecalLocation);
	DOREPLIFETIME(ADestroypack, DecalRotation);
}

float ADestroypack::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (Role == ROLE_Authority)
	{
		Health -= DamageAmount;
		OnRep_UpdateHealth();
		if (Health <= 0.f)
		{
			IsDestroyed = true;
			OnRep_IsDestroyed();
		}
	}
	return DamageAmount;
}

void ADestroypack::OnRep_UpdateHealth()
{
	TextRender->SetText(FText::FromString(FString::SanitizeFloat(Health)));
}

void ADestroypack::OnRep_IsDestroyed()
{
	Destroy(DefaultDamage, DestructibleComponent->GetComponentLocation(), DestructibleComponent->GetForwardVector(), DefaultImpulse);
}

void ADestroypack::Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	DestructibleComponent->ApplyDamage(Damage, HitLocation, ImpulseDir, Impulse);
	TextRender->SetText(FText::FromString("Object Destroyed!"));
	CollisionSphere->DestroyComponent(true);
	BloodDecal->SetWorldRotation(DecalRotation);
	BloodDecal->SetWorldLocation(DecalLocation);
	BloodDecal->SetHiddenInGame(false);
}

void ADestroypack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role = ROLE_Authority)
	{
		FVector ProjectileDirection = OtherComp->GetForwardVector();
		FHitResult* HitResult = new FHitResult();
		FVector StartTrace = RootComponent->GetComponentLocation() + ProjectileDirection * 65;
		FVector RaycastDirection = ProjectileDirection;
		FVector EndTrace = (StartTrace + (RaycastDirection * 1000.f));
		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
		{
			DecalLocation = HitResult->Location;
			DecalRotation = ProjectileDirection.Rotation();
		}
	}
}