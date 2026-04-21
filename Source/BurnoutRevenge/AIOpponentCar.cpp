#include "AIOpponentCar.h"
#include "ChaosVehicleMovementComponent.h"

AAIOpponentCar::AAIOpponentCar()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAIOpponentCar::BeginPlay()
{
	Super::BeginPlay();
	CurrentWaypoint = StartWaypoint;
}

void AAIOpponentCar::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (!CurrentWaypoint)
		return;

	FVector Target = CurrentWaypoint->GetActorLocation();
	SteerTowardTarget(Target);

	if (FVector::DistSquared(GetActorLocation(), Target) < WaypointAcceptRadius * WaypointAcceptRadius)
		CurrentWaypoint = CurrentWaypoint->NextWaypoint;
}

void AAIOpponentCar::SteerTowardTarget(FVector TargetLocation)
{
	UChaosVehicleMovementComponent* VM = GetVehicleMovementComponent();
	if (!VM)
		return;

	FVector ToTarget = (TargetLocation - GetActorLocation()).GetSafeNormal();
	float SteerDot = FVector::DotProduct(GetActorRightVector(), ToTarget);
	float ForwardDot = FVector::DotProduct(GetActorForwardVector(), ToTarget);

	VM->SetThrottleInput(ForwardDot > 0.f ? 1.f : 0.f);
	VM->SetBrakeInput(ForwardDot < -0.3f ? 1.f : 0.f);
	VM->SetSteeringInput(FMath::Clamp(SteerDot * SteerStrength, -1.f, 1.f));
}
