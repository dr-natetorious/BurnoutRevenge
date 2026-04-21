#include "AIOpponentCar.h"
#include "ChaosVehicleMovementComponent.h"

AAIOpponentCar::AAIOpponentCar()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIOpponentCar::BeginPlay()
{
	Super::BeginPlay();
}

void AAIOpponentCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Target = GetNextWaypoint();
	if (!Target)
		return;

	SteerTowardTarget(Target->GetActorLocation(), DeltaTime);

	// Advance waypoint when close enough
	float DistSq = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSq < 250000.f && Waypoints.Num() > 0)
		CurrentWaypoint = (CurrentWaypoint + 1) % Waypoints.Num();
}

void AAIOpponentCar::SteerTowardTarget(FVector TargetLocation, float DeltaTime)
{
	UChaosVehicleMovementComponent* VM = GetVehicleMovementComponent();
	if (!VM)
		return;

	FVector ToTarget = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	float SteerDot = FVector::DotProduct(Right, ToTarget);
	float ForwardDot = FVector::DotProduct(Forward, ToTarget);

	VM->SetThrottleInput(ForwardDot > 0.f ? 1.f : 0.f);
	VM->SetBrakeInput(ForwardDot < -0.3f ? 1.f : 0.f);
	VM->SetSteeringInput(FMath::Clamp(SteerDot * SteerStrength, -1.f, 1.f));
}

AActor* AAIOpponentCar::GetNextWaypoint() const
{
	if (Waypoints.IsEmpty())
		return nullptr;
	return Waypoints[CurrentWaypoint % Waypoints.Num()];
}
