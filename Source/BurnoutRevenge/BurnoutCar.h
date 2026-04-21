#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BurnoutCar.generated.h"

class UCrashCameraComponent;
class UNearMissComponent;

UCLASS()
class BURNOUTREVENGE_API ABurnoutCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ABurnoutCar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCrashCameraComponent* CrashCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNearMissComponent* NearMiss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostForce = 120000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostMaxCharge = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostDrainRate = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boost")
	float BoostChargePerNearMiss = 20.f;

	UPROPERTY(BlueprintReadOnly, Category = "Boost")
	float BoostCharge = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Boost")
	bool bIsBoosting = false;

	UFUNCTION(BlueprintCallable, Category = "Boost")
	void AddBoostCharge(float Amount);

protected:
	virtual void BeginPlay() override;

private:
	void OnBoostPressed();
	void OnBoostReleased();
};
