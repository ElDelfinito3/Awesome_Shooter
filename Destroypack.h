#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "Destroypack.generated.h"

UCLASS()
class DZ_SHOOTER_API ADestroypack : public AActor
{
	GENERATED_BODY()
	
public:	

	ADestroypack();

	//required network scaffolding
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);

public:	

	UPROPERTY()
	class USceneComponent* Root;

	UPROPERTY()
	class USphereComponent* CollisionSphere;

	UPROPERTY()
	class UTextRenderComponent* TextRender;

	UPROPERTY(EditAnywhere, Category = "DestroypackData")
	class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(EditAnywhere, Category = "DestroypackData")
	class UDecalComponent* BloodDecal;

	UPROPERTY(EditAnywhere, Category = "DestroypackData")
	float FullHealth;

	UPROPERTY()
	float DefaultDamage;
	
	UPROPERTY()
	float DefaultImpulse;

	UPROPERTY(Replicated)
	FVector DecalLocation;

	UPROPERTY(Replicated)
	FRotator DecalRotation;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateHealth)
	float Health;

	UPROPERTY(ReplicatedUsing = OnRep_IsDestroyed)
	bool IsDestroyed;

	UFUNCTION()
	virtual void OnRep_UpdateHealth();

	UFUNCTION()
	virtual void OnRep_IsDestroyed();

	UFUNCTION()
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
