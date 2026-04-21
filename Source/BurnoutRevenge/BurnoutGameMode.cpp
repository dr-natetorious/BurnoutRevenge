#include "BurnoutGameMode.h"
#include "BurnoutCar.h"

ABurnoutGameMode::ABurnoutGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = ABurnoutCar::StaticClass();
}

void ABurnoutGameMode::BeginPlay()
{
	Super::BeginPlay();
	TimeRemaining = RoadRageDuration;
	bGameStarted = true;
}

void ABurnoutGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameStarted || bGameOver)
		return;

	if (GameType == EBurnoutGameType::RoadRage)
	{
		TimeRemaining = FMath::Max(0.f, TimeRemaining - DeltaTime);
		if (TimeRemaining <= 0.f)
			bGameOver = true;
	}
}

void ABurnoutGameMode::RegisterTakedown()
{
	TakedownCount++;
}

void ABurnoutGameMode::RegisterLapComplete(int32 CurrentLap)
{
	if (GameType == EBurnoutGameType::Race && CurrentLap >= TotalLaps)
		bGameOver = true;
}
