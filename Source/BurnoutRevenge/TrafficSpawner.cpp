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

	SpawnTrafficVehicle();
}

void ATrafficSpawner::SpawnTrafficVehicle()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player || !TrafficClass)
		return;

	// Spawn ahead of player in oncoming lane
	FVector Forward = Player->GetActorForwardVector();
	FVector Right = Player->GetActorRightVector();
	FVector SpawnPos = Player->GetActorLocation()
		+ Forward * SpawnAheadDistance
		- Right * LaneWidth;  // oncoming lane

	FRotator SpawnRot = (-Forward).Rotation();  // facing player

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	GetWorld()->SpawnActor<AActor>(TrafficClass, SpawnPos, SpawnRot, Params);
}
