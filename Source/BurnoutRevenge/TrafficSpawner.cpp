#include "TrafficSpawner.h"
#include "TrafficVehicle.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

ATrafficSpawner::ATrafficSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrafficSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ATrafficSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer += DeltaTime;
	if (SpawnTimer < SpawnInterval)
		return;

	SpawnTimer = 0.f;

	TArray<AActor*> Existing;
	UGameplayStatics::GetAllActorsOfClass(this, TrafficClass, Existing);
	if (Existing.Num() >= MaxTrafficCount)
		return;

	SpawnTrafficVehicle(true);   // oncoming
	SpawnTrafficVehicle(false);  // same direction
}

void ATrafficSpawner::SpawnTrafficVehicle(bool bOncoming)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player || !TrafficClass)
		return;

	FVector Forward = Player->GetActorForwardVector();
	FVector Right = Player->GetActorRightVector();

	float AheadDist = bOncoming ? SpawnAheadDistance : -SpawnAheadDistance * 0.5f;
	float LaneSide = bOncoming ? -LaneWidth : LaneWidth;

	FVector SpawnPos = Player->GetActorLocation()
		+ Forward * AheadDist
		+ Right * LaneSide;
	SpawnPos.Z += 100.f;

	FRotator SpawnRot = bOncoming ? (-Forward).Rotation() : Forward.Rotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AActor>(TrafficClass, SpawnPos, SpawnRot, Params);
}
