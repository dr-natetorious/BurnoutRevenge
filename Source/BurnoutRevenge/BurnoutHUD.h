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

private:
	void DrawBar(float X, float Y, float Width, float Height, float Fill, FLinearColor Color);
};
