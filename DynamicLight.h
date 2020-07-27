#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicLight.generated.h"

#pragma region ENUMS

UENUM()
enum ActiveLight { White, Yellow, Blue, Red, Green };

#pragma endregion ENUMS

UCLASS()
class DZ_SHOOTER_API ADynamicLight : public AActor
{
	GENERATED_BODY()

public:	

	ADynamicLight();

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SwitchVisibilityTrigger;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* YellowTrigger;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BlueTrigger;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* RedTrigger;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* GreenTrigger;

	UPROPERTY(ReplicatedUsing = OnRep_InitLightColor)
	TEnumAsByte<ActiveLight> LightColor;

	UPROPERTY(ReplicatedUsing = OnRep_InitLightVisibility)
	bool IsLightActive;

protected:

	virtual void BeginPlay() override;

public:

	//required network scaffolding
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_InitLightVisibility();

	UFUNCTION()
	virtual void OnRep_InitLightColor();

	UFUNCTION()
	void SwitchLightVisibility(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
	UFUNCTION()
	void SetLightToYellow(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetLightToBlue(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetLightToRed(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetLightToGreen(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};