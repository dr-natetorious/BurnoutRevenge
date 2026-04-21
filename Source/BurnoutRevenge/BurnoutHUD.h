#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BurnoutGameState.h"
#include "BurnoutHUD.generated.h"

UCLASS()
class BURNOUTREVENGE_API ABurnoutHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void FlashNearMiss();
	void FlashTakedown();

private:
	void DrawBar(float X, float Y, float Width, float Height, float Fill, FLinearColor Color);

	float NearMissFlashTimer = 0.f;
	float TakedownFlashTimer = 0.f;
	int32 LastTakedownCount = 0;
	int32 LastNearMissCount = 0;
};
