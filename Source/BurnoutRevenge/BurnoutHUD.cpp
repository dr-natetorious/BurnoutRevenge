#include "BurnoutHUD.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "ChaosVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void ABurnoutHUD::DrawHUD()
{
	Super::DrawHUD();

	ABurnoutCar* Car = Cast<ABurnoutCar>(GetOwningPawn());
	ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!Car || !Canvas)
		return;

	float W = Canvas->SizeX;
	float H = Canvas->SizeY;

	// Speed
	float SpeedKMH = 0.f;
	if (UChaosVehicleMovementComponent* VM = Car->GetVehicleMovementComponent())
		SpeedKMH = VM->GetForwardSpeed() * 0.036f;

	FString SpeedStr = FString::Printf(TEXT("%d km/h"), FMath::RoundToInt(SpeedKMH));
	DrawText(SpeedStr, FLinearColor::White, W - 220.f, H - 80.f, GEngine->GetLargeFont(), 1.5f);

	// Boost bar
	float BoostFill = Car->BoostMaxCharge > 0.f ? Car->BoostCharge / Car->BoostMaxCharge : 0.f;
	FLinearColor BoostColor = Car->bIsBoosting ? FLinearColor(1.f, 0.5f, 0.f) : FLinearColor(0.f, 0.8f, 1.f);
	DrawText(TEXT("BOOST"), FLinearColor::White, W - 220.f, H - 120.f, nullptr, 1.f);
	DrawBar(W - 220.f, H - 105.f, 180.f, 18.f, BoostFill, BoostColor);

	if (!GM)
		return;

	// Takedown counter
	FString TakedownStr = FString::Printf(TEXT("TAKEDOWNS: %d"), GM->TakedownCount);
	DrawText(TakedownStr, FLinearColor(1.f, 0.3f, 0.f), 30.f, 30.f, GEngine->GetLargeFont(), 1.2f);

	// Timer (Road Rage) or Lap (Race)
	if (GM->GameType == EBurnoutGameType::RoadRage)
	{
		int32 Secs = FMath::CeilToInt(GM->TimeRemaining);
		FString TimeStr = FString::Printf(TEXT("%d:%02d"), Secs / 60, Secs % 60);
		DrawText(TimeStr, Secs < 30 ? FLinearColor::Red : FLinearColor::White, W / 2.f - 40.f, 30.f, GEngine->GetLargeFont(), 1.5f);
	}

	if (GM->bGameOver)
		DrawText(TEXT("GAME OVER"), FLinearColor::Red, W / 2.f - 120.f, H / 2.f - 30.f, GEngine->GetLargeFont(), 2.5f);
}

void ABurnoutHUD::DrawBar(float X, float Y, float Width, float Height, float Fill, FLinearColor Color)
{
	DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), X, Y, Width, Height);
	DrawRect(Color, X + 2.f, Y + 2.f, (Width - 4.f) * Fill, Height - 4.f);
}
