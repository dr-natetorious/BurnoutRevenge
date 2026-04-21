#include "BurnoutGameMode.h"
#include "BurnoutGameState.h"
#include "BurnoutCar.h"
#include "BurnoutHUD.h"
#include "Kismet/GameplayStatics.h"

ABurnoutGameMode::ABurnoutGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = ABurnoutCar::StaticClass();
	GameStateClass = ABurnoutGameState::StaticClass();
	HUDClass = ABurnoutHUD::StaticClass();
}

void ABurnoutGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ABurnoutGameState* GS = GetGameState<ABurnoutGameState>())
	{
		GS->TimeRemaining = RoadRageDuration;
		GS->bGameOver = false;
		GS->TakedownCount = 0;
		GS->NearMissCount = 0;
	}

	bGameStarted = true;
}

void ABurnoutGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameStarted)
		return;

	ABurnoutGameState* GS = GetGameState<ABurnoutGameState>();
	if (!GS || GS->bGameOver)
		return;

	if (GameType == EBurnoutGameType::RoadRage)
	{
		GS->TimeRemaining = FMath::Max(0.f, GS->TimeRemaining - DeltaTime);
		if (GS->TimeRemaining <= 0.f)
			EndGame();
	}
}

void ABurnoutGameMode::RegisterTakedown()
{
	if (ABurnoutGameState* GS = GetGameState<ABurnoutGameState>())
		GS->TakedownCount++;
}

void ABurnoutGameMode::RegisterNearMiss()
{
	if (ABurnoutGameState* GS = GetGameState<ABurnoutGameState>())
		GS->NearMissCount++;
}

void ABurnoutGameMode::RegisterLapComplete(int32 CurrentLap)
{
	if (GameType == EBurnoutGameType::Race && CurrentLap >= TotalLaps)
		EndGame();
}

void ABurnoutGameMode::EndGame()
{
	if (ABurnoutGameState* GS = GetGameState<ABurnoutGameState>())
		GS->bGameOver = true;
}

void ABurnoutGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}
