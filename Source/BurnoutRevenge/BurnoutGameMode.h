#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BurnoutGameMode.generated.h"

UENUM(BlueprintType)
enum class EBurnoutGameType : uint8
{
	Race,
	RoadRage
};

UCLASS()
class BURNOUTREVENGE_API ABurnoutGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABurnoutGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	EBurnoutGameType GameType = EBurnoutGameType::RoadRage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float RoadRageDuration = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 TotalLaps = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	int32 TakedownCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	float TimeRemaining = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool bGameOver = false;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RegisterTakedown();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RegisterLapComplete(int32 CurrentLap);

private:
	bool bGameStarted = false;
};
