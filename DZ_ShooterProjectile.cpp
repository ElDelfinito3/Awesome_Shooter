// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DZ_ShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ADZ_ShooterProjectile::ADZ_ShooterProjectile() 
{
	// Use a sphere as a simple collision representation
	{
		CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
		CollisionComp->InitSphereRadius(5.0f);
		CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
		CollisionComp->OnComponentHit.AddDynamic(this, &ADZ_ShooterProjectile::OnHit);		// set up a notification for when this component hits something blocking
	}

	// Players can't walk on it
	{
		CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
		CollisionComp->CanCharacterStepUpOn = ECB_No;
	}

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileMovement->UpdatedComponent = CollisionComp;
		ProjectileMovement->InitialSpeed = 3000.f;
		ProjectileMovement->MaxSpeed = 3000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;
	}

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Damage = 20.f;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ADZ_ShooterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->GetCollisionObjectType() == ECC_PhysicsBody)
	{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		if (Role == ROLE_Authority)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorLocation(), MyHit, nullptr, this, FireDamageType);
			Destroy();
		}
	}
}