#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BurnoutGameState.generated.h"

UCLASS()
class BURNOUTREVENGE_API ABurnoutGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 TakedownCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 NearMissCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	float TimeRemaining = 180.f;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	bool bGameOver = false;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const { return TakedownCount * 1000 + NearMissCount * 100; }
};
