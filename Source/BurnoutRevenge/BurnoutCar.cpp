#include "BurnoutCar.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/InputComponent.h"

ABurnoutCar::ABurnoutCar()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABurnoutCar::BeginPlay()
{
	Super::BeginPlay();
	BoostCharge = 0.f;
}

void ABurnoutCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBoosting && BoostCharge > 0.f)
	{
		if (UChaosVehicleMovementComponent* VM = GetVehicleMovementComponent())
		{
			FVector BoostDir = GetActorForwardVector();
			GetMesh()->AddForce(BoostDir * BoostForce, NAME_None, true);
		}

		BoostCharge = FMath::Max(0.f, BoostCharge - BoostDrainRate * DeltaTime);

		if (BoostCharge <= 0.f)
			bIsBoosting = false;
	}
}

void ABurnoutCar::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Boost", IE_Pressed, this, &ABurnoutCar::OnBoostPressed);
	InputComponent->BindAction("Boost", IE_Released, this, &ABurnoutCar::OnBoostReleased);
}

void ABurnoutCar::AddBoostCharge(float Amount)
{
	BoostCharge = FMath::Min(BoostMaxCharge, BoostCharge + Amount);
}

void ABurnoutCar::OnBoostPressed()
{
	if (BoostCharge > 0.f)
		bIsBoosting = true;
}

void ABurnoutCar::OnBoostReleased()
{
	bIsBoosting = false;
}
