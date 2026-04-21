#include "BurnoutHUD.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "BurnoutGameState.h"
#include "ChaosVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void ABurnoutHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
		return;

	ABurnoutCar* Car = Cast<ABurnoutCar>(GetOwningPawn());
	ABurnoutGameState* GS = GetWorld()->GetGameState<ABurnoutGameState>();

	float W = Canvas->SizeX;
	float H = Canvas->SizeY;

	if (Car)
	{
		// Speed
		float SpeedKMH = 0.f;
		if (UChaosVehicleMovementComponent* VM = Car->GetVehicleMovementComponent())
			SpeedKMH = FMath::Abs(VM->GetForwardSpeed()) * 0.036f;

		DrawText(FString::Printf(TEXT("%d km/h"), FMath::RoundToInt(SpeedKMH)),
			FLinearColor::White, W - 220.f, H - 80.f, GEngine->GetLargeFont(), 1.5f);

		// Boost bar
		float Fill = Car->BoostMaxCharge > 0.f ? Car->BoostCharge / Car->BoostMaxCharge : 0.f;
		FLinearColor BoostColor = Car->bIsBoosting ? FLinearColor(1.f, 0.5f, 0.f) : FLinearColor(0.f, 0.8f, 1.f);
		DrawText(TEXT("BOOST"), FLinearColor::White, W - 220.f, H - 120.f, nullptr, 1.f);
		DrawBar(W - 220.f, H - 105.f, 180.f, 18.f, Fill, BoostColor);
	}

	if (!GS)
		return;

	// Takedowns + score
	DrawText(FString::Printf(TEXT("TAKEDOWNS: %d"), GS->TakedownCount),
		FLinearColor(1.f, 0.3f, 0.f), 30.f, 30.f, GEngine->GetLargeFont(), 1.2f);
	DrawText(FString::Printf(TEXT("SCORE: %d"), GS->GetScore()),
		FLinearColor::White, 30.f, 60.f, nullptr, 1.f);

	// Timer
	ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM && GM->GameType == EBurnoutGameType::RoadRage)
	{
		int32 Secs = FMath::CeilToInt(GS->TimeRemaining);
		FString TimeStr = FString::Printf(TEXT("%d:%02d"), Secs / 60, Secs % 60);
		FLinearColor TimeColor = Secs < 30 ? FLinearColor::Red : FLinearColor::White;
		DrawText(TimeStr, TimeColor, W / 2.f - 40.f, 30.f, GEngine->GetLargeFont(), 1.5f);
	}

	// Game over
	if (GS->bGameOver)
	{
		DrawText(TEXT("GAME OVER"), FLinearColor::Red, W / 2.f - 150.f, H / 2.f - 50.f, GEngine->GetLargeFont(), 3.f);
		DrawText(FString::Printf(TEXT("SCORE: %d"), GS->GetScore()),
			FLinearColor::White, W / 2.f - 80.f, H / 2.f + 20.f, GEngine->GetLargeFont(), 1.5f);
		DrawText(TEXT("Press R to Restart"), FLinearColor::White, W / 2.f - 100.f, H / 2.f + 70.f, nullptr, 1.f);
	}
}

void ABurnoutHUD::DrawBar(float X, float Y, float Width, float Height, float Fill, FLinearColor Color)
{
	DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), X, Y, Width, Height);
	DrawRect(Color, X + 2.f, Y + 2.f, (Width - 4.f) * Fill, Height - 4.f);
}
