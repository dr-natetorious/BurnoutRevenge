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
	ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	float W = Canvas->SizeX;
	float H = Canvas->SizeY;
	float DT = GetWorld()->GetDeltaSeconds();

	// Detect new events and trigger flashes
	if (GS)
	{
		if (GS->TakedownCount > LastTakedownCount) { TakedownFlashTimer = 1.f; LastTakedownCount = GS->TakedownCount; }
		if (GS->NearMissCount > LastNearMissCount) { NearMissFlashTimer = 0.5f; LastNearMissCount = GS->NearMissCount; }
		TakedownFlashTimer = FMath::Max(0.f, TakedownFlashTimer - DT);
		NearMissFlashTimer = FMath::Max(0.f, NearMissFlashTimer - DT);
	}

	// Near miss flash
	if (NearMissFlashTimer > 0.f)
		DrawText(TEXT("NEAR MISS!"), FLinearColor(1.f, 1.f, 0.f, NearMissFlashTimer * 2.f),
			W / 2.f - 80.f, H / 2.f - 80.f, GEngine->GetLargeFont(), 1.8f);

	// Takedown flash
	if (TakedownFlashTimer > 0.f)
		DrawText(TEXT("TAKEDOWN!"), FLinearColor(1.f, 0.2f, 0.f, TakedownFlashTimer),
			W / 2.f - 80.f, H / 2.f - 110.f, GEngine->GetLargeFont(), 2.f);

	if (Car)
	{
		float SpeedKMH = 0.f;
		if (UChaosVehicleMovementComponent* VM = Car->GetVehicleMovementComponent())
			SpeedKMH = FMath::Abs(VM->GetForwardSpeed()) * 0.036f;

		DrawText(FString::Printf(TEXT("%d km/h"), FMath::RoundToInt(SpeedKMH)),
			FLinearColor::White, W - 220.f, H - 80.f, GEngine->GetLargeFont(), 1.5f);

		float Fill = Car->BoostMaxCharge > 0.f ? Car->BoostCharge / Car->BoostMaxCharge : 0.f;
		FLinearColor BoostColor = Car->bIsBoosting ? FLinearColor(1.f, 0.5f, 0.f) : FLinearColor(0.f, 0.8f, 1.f);
		DrawText(TEXT("BOOST"), FLinearColor::White, W - 220.f, H - 120.f, nullptr, 1.f);
		DrawBar(W - 220.f, H - 105.f, 180.f, 18.f, Fill, BoostColor);
	}

	if (!GS)
		return;

	DrawText(FString::Printf(TEXT("TAKEDOWNS: %d"), GS->TakedownCount),
		FLinearColor(1.f, 0.3f, 0.f), 30.f, 30.f, GEngine->GetLargeFont(), 1.2f);
	DrawText(FString::Printf(TEXT("SCORE: %d"), GS->GetScore()),
		FLinearColor::White, 30.f, 60.f, nullptr, 1.f);

	if (GM && GM->GameType == EBurnoutGameType::RoadRage)
	{
		int32 Secs = FMath::CeilToInt(GS->TimeRemaining);
		DrawText(FString::Printf(TEXT("%d:%02d"), Secs / 60, Secs % 60),
			Secs < 30 ? FLinearColor::Red : FLinearColor::White,
			W / 2.f - 40.f, 30.f, GEngine->GetLargeFont(), 1.5f);
	}

	if (GS->bGameOver)
	{
		DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.6f), 0.f, 0.f, W, H);
		DrawText(TEXT("GAME OVER"), FLinearColor::Red, W / 2.f - 150.f, H / 2.f - 60.f, GEngine->GetLargeFont(), 3.f);
		DrawText(FString::Printf(TEXT("SCORE: %d"), GS->GetScore()),
			FLinearColor::White, W / 2.f - 80.f, H / 2.f + 20.f, GEngine->GetLargeFont(), 1.5f);
		DrawText(FString::Printf(TEXT("TAKEDOWNS: %d"), GS->TakedownCount),
			FLinearColor(1.f, 0.3f, 0.f), W / 2.f - 80.f, H / 2.f + 60.f, nullptr, 1.f);
		DrawText(TEXT("Press R to Restart"), FLinearColor::White, W / 2.f - 100.f, H / 2.f + 90.f, nullptr, 1.f);
	}
}

void ABurnoutHUD::DrawBar(float X, float Y, float Width, float Height, float Fill, FLinearColor Color)
{
	DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f), X, Y, Width, Height);
	DrawRect(Color, X + 2.f, Y + 2.f, (Width - 4.f) * Fill, Height - 4.f);
}

void ABurnoutHUD::FlashNearMiss() { NearMissFlashTimer = 0.5f; }
void ABurnoutHUD::FlashTakedown() { TakedownFlashTimer = 1.f; }
